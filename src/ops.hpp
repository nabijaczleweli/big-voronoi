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


#pragma once


#include "util.hpp"
#include <SFML/Graphics.hpp>
#include <nonstd/variant.hpp>
#include <ostream>
#include <pb-cpp/multibar.hpp>


namespace big_voronoi {
	struct job_context {
		point_3d size;
		std::vector<point_3d> points;
		std::vector<sf::Color> colours;
	};

	std::ostream & operator<<(std::ostream & out, const job_context & job);


	using job_signature = void (*)(job_context ctx, sf::Image * where, std::size_t how_many, std::size_t z0, std::size_t thread_idx, pb::progressbar progress);

	extern const std::chrono::milliseconds progressbar_max_refresh_rate;
	extern const std::vector<sf::Color> default_colours;


	std::vector<sf::Color> generate_colours(std::size_t how_many);

	std::vector<point_3d> generate_points(point_3d size, std::size_t how_many);
	void highlight_points(const job_context & ctx, std::vector<sf::Image> & images);

	void run_jobs(job_signature job, const std::string & job_name, const job_context & ctx, std::size_t jobs, sf::Image * images, pb::multibar & progresses);
	void run_jobs(job_signature job, const char * job_name, const job_context & ctx, std::size_t jobs, sf::Image * images, pb::multibar & progresses);

	void colour_layers_job(job_context ctx, sf::Image * where, std::size_t how_many, std::size_t z0, std::size_t thread_idx, pb::progressbar progress);

	std::string filename_to_save(const point_3d & size, std::size_t z);

	double distance(const point_3d & from, const point_3d & to);
}
