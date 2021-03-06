// The MIT License (MIT)

// Copyright (c) 2018 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "ops.hpp"
#include "assets.hpp"
#include <algorithm>
#include <iomanip>
#include <random>
#include <seed11/seed11.hpp>
#include <sstream>
#include <thread>


const std::chrono::milliseconds big_voronoi::progressbar_max_refresh_rate{100};
const std::vector<sf::Color> big_voronoi::default_colours = []() {
	std::istringstream ss{assets::default_colours_s};
	return read_data(parse_colour, ss);
}();


std::ostream & big_voronoi::operator<<(std::ostream & out, const job_context & job) {
	out << "Size: " << std::get<0>(job.size) << 'x' << std::get<1>(job.size) << 'x' << std::get<2>(job.size) << '\n';

	out << "Point set:\n";
	for(auto && p : job.points)
		out << "\t[" << std::get<0>(p) << ", " << std::get<1>(p) << ", " << std::get<2>(p) << "]\n";

	out << "Colours:\n" << std::hex << std::setfill('0');
	for(auto && c : job.colours)
		out << "\t#" << std::setw(2) << static_cast<unsigned int>(c.r) << std::setw(2) << static_cast<unsigned int>(c.g) << std::setw(2)
		    << static_cast<unsigned int>(c.b) << '\n';
	out << std::dec;

	out << '\n';
	return out;
}

std::vector<sf::Color> big_voronoi::generate_colours(std::size_t how_many) {
	auto rng = seed11::make_seeded<std::mt19937>();
	std::uniform_int_distribution<unsigned int> rgb_dist{0, 255};  // Can't be std::uint8_t because C++ is good

	std::vector<sf::Color> out{how_many};
	for(auto && p : out)
		p = {static_cast<std::uint8_t>(rgb_dist(rng)), static_cast<std::uint8_t>(rgb_dist(rng)), static_cast<std::uint8_t>(rgb_dist(rng))};
	return out;
}

std::vector<big_voronoi::point_3d> big_voronoi::generate_points(point_3d size, std::size_t how_many) {
	auto rng = seed11::make_seeded<std::mt19937>();
	std::uniform_int_distribution<std::size_t> x_dist(0, std::get<0>(size) - 1);
	std::uniform_int_distribution<std::size_t> y_dist(0, std::get<1>(size) - 1);
	std::uniform_int_distribution<std::size_t> z_dist(0, std::get<2>(size) - 1);

	std::vector<point_3d> out{how_many};
	for(auto && p : out)
		p = std::make_tuple(x_dist(rng), y_dist(rng), z_dist(rng));
	return out;
}

void big_voronoi::highlight_points(const job_context & ctx, std::vector<sf::Image> & images) {
	for(std::size_t i = 0; i < ctx.points.size(); ++i) {
		auto && p = ctx.points[i];
		images[std::get<2>(p)].setPixel(std::get<0>(p), std::get<1>(p), invert_colour(ctx.colours[i]));
	}
}

void big_voronoi::run_jobs(job_signature job, const std::string & job_name, const job_context & ctx, std::size_t jobs, sf::Image * images,
                           pb::multibar & progresses) {
	const auto per_layer             = std::get<2>(ctx.size) / jobs;
	const auto additional_last_layer = std::get<2>(ctx.size) - (per_layer * jobs);  // Integer arithmetic is fun


	progresses.println(capitalise_first(job_name) + ":");

	for(std::size_t i = 0; i < jobs; ++i) {
		const auto layer_count = per_layer + (i + 1 == jobs ? additional_last_layer : 0);
		std::thread(job, ctx, &images[i * per_layer], layer_count, i * per_layer, i, progresses.create_bar(layer_count)).detach();
	}

	progresses.println();
	progresses.listen();
}

void big_voronoi::run_jobs(job_signature job, const char * job_name, const job_context & ctx, std::size_t jobs, sf::Image * images, pb::multibar & progresses) {
	run_jobs(job, std::string{job_name}, ctx, jobs, images, progresses);
}


void big_voronoi::colour_layers_job(job_context ctx, sf::Image * where, std::size_t how_many, std::size_t z0, std::size_t thread_idx,
                                    pb::progressbar progress) {
	progress.message("Thread #" + std::to_string(thread_idx + 1) + " [layers] ");

	for(std::size_t z = 0; z < how_many; ++z) {
		for(auto y = 0u; y < std::get<1>(ctx.size); ++y)
			for(auto x = 0u; x < std::get<0>(ctx.size); ++x) {
				point_3d this_point{x, y, z + z0};
				where[z].setPixel(
				    x, y,
				    ctx.colours[std::max_element(std::begin(ctx.points), std::end(ctx.points),
				                                 [&](const auto & lhs, const auto & rhs) { return distance(lhs, this_point) < distance(rhs, this_point); }) -
				                std::begin(ctx.points)]);
			}

		++progress;
	}

	progress.finish();
}

std::string big_voronoi::filename_to_save(const point_3d & size, std::size_t z) {
	std::ostringstream out;
	out << "voronoi-" << std::get<0>(size) << 'x' << std::get<1>(size) << 'x' << std::get<2>(size) << '-' << std::setfill('0') << std::setw(5) << z << ".png";
	return out.str();
}

double big_voronoi::distance(const point_3d & from, const point_3d & to) {
	const auto dx = abs_diff(std::get<0>(from), std::get<0>(to));
	const auto dy = abs_diff(std::get<1>(from), std::get<1>(to));
	const auto dz = abs_diff(std::get<2>(from), std::get<2>(to));

	return std::sqrt(dx * dx + dy * dy + dz * dz);
}
