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
#include "options/options.hpp"
#include "util.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


int main(int argc, const char ** argv) {
	const auto opts_r = big_voronoi::options::parse(argc, argv);
	if(const auto error_val = nonstd::get_if<big_voronoi::option_err>(&opts_r)) {
		std::cerr << error_val->second << '\n';
		return error_val->first;
	}
	const auto opts = std::move(nonstd::get<big_voronoi::options>(opts_r));


	std::vector<sf::Color> colours;
	if(opts.colours)
		if(const auto colour_count = nonstd::get_if<std::size_t>(&*opts.colours)) {
			colours.insert(colours.end(), std::begin(big_voronoi::default_colours),
			               std::begin(big_voronoi::default_colours) + std::min(big_voronoi::default_colours.size(), *colour_count));
			if(colours.size() < *colour_count) {
				const auto new_colours = big_voronoi::generate_colours(*colour_count - colours.size());
				colours.insert(colours.end(), std::begin(new_colours), std::end(new_colours));
			}
		} else {
			std::ifstream colours_in{nonstd::get<std::string>(*opts.colours)};
			colours = big_voronoi::read_data(big_voronoi::parse_colour, colours_in);
		}
	else
		colours = big_voronoi::default_colours;

	if(colours.empty()) {
		std::cerr << "No colours, can't generate voronoi.\n";
		return 3;
	}

	std::vector<big_voronoi::point_3d> points;
	if(opts.points)
		if(const auto point_count = nonstd::get_if<std::size_t>(&*opts.points))
			points = big_voronoi::generate_points(opts.size, std::min(*point_count, colours.size()));
		else {
			std::ifstream points_in{nonstd::get<std::string>(*opts.points)};
			points = big_voronoi::read_data(big_voronoi::parse_point, points_in);
		}
	else
		points = big_voronoi::generate_points(opts.size, colours.size());

	if(points.empty()) {
		std::cerr << "No points, can't generate voronoi.\n";
		return 4;
	}

	points.resize(std::min(points.size(), colours.size()));
	colours.resize(points.size());


	std::cout << "Allocating " << big_voronoi::separated_number(std::get<0>(opts.size) * std::get<1>(opts.size) * std::get<2>(opts.size) * 4 / 1024)
	          << "KiB of images..." << std::flush;

	std::vector<sf::Image> images{std::get<2>(opts.size)};
	for(auto && img : images)
		img.create(std::get<0>(opts.size), std::get<1>(opts.size));

	std::cout << " Done!\n\n";


	big_voronoi::job_context ctx{opts.size, std::move(points), std::move(colours)};
	std::cout << "Configuration:\n" << ctx;

	pb::multibar progresses;
	big_voronoi::run_jobs(big_voronoi::colour_layers_job, "generation on " + std::to_string(opts.jobs) + " thread" + (opts.jobs == 1 ? "" : "s"), ctx, opts.jobs,
	                      images.data(), progresses);
	big_voronoi::highlight_points(ctx, images);

	pb::progressbar progress(images.size());
	progress.message("Saving into " + (opts.out_directory.empty() ? std::string{"./"} : opts.out_directory) + ": ");

	for(std::size_t i = 0; i < images.size(); ++i) {
		images[i].saveToFile(opts.out_directory + big_voronoi::filename_to_save(opts.size, i));
		++progress;
	}
}
