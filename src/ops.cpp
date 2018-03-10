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
#include <iomanip>
#include <sstream>
#include <thread>


using namespace std::literals;


const std::chrono::milliseconds big_voronoi::progressbar_max_refresh_rate = 100ms;

const std::vector<point_3d> default_colourset{};


void big_voronoi::run_jobs(job_signature job, const std::string & job_name, const point_3d & size, std::size_t jobs, sf::Image * images,
                           pb::multibar & progresses) {
	const auto per_layer             = std::get<2>(size) / jobs;
	const auto additional_last_layer = std::get<2>(size) - (per_layer * jobs);  // Integer arithmetic is fun


	progresses.println(capitalise_first(job_name) + ":");

	for(std::size_t i = 0; i < jobs; ++i) {
		const auto layer_count = per_layer + (i + 1 == jobs ? additional_last_layer : 0);
		std::thread(job, size, &images[i * per_layer], layer_count, i, progresses.create_bar(layer_count)).detach();
	}

	progresses.println();
	progresses.listen();
}

void big_voronoi::run_jobs(job_signature job, const char * job_name, const point_3d & size, std::size_t jobs, sf::Image * images, pb::multibar & progresses) {
	run_jobs(job, std::string{job_name}, size, jobs, images, progresses);
}


void big_voronoi::colour_layers_job(point_3d size, sf::Image * where, std::size_t how_many, std::size_t thread_idx, pb::progressbar progress) {
	progress.message("Thread #" + std::to_string(thread_idx + 1) + " [layers] ");

	for(std::size_t z = 0; z < how_many; ++z) {
		for(auto y = 0u; y < std::get<1>(size); ++y)
			for(auto x = 0u; x < std::get<0>(size); ++x)
				where[z].setPixel(x, y, sf::Color{static_cast<std::uint8_t>(x % 256), static_cast<std::uint8_t>(y % 256), static_cast<std::uint8_t>(z % 256)});

		++progress;
	}

	progress.finish();
}

std::string big_voronoi::filename_to_save(const point_3d & size, std::size_t z) {
	std::ostringstream out;
	out << "voronoi-" << std::get<0>(size) << 'x' << std::get<1>(size) << 'x' << std::get<2>(size) << '-' << std::setfill('0') << std::setw(5) << z << ".png";
	return out.str();
}
