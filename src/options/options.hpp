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


#include "../util.hpp"
#include <nonstd/optional.hpp>
#include <nonstd/variant.hpp>
#include <utility>


namespace big_voronoi {
	using option_err = std::pair<int, std::string>;

	/// Representation of command-line configurable application parameters.
	struct options {
		/// The output size of the voronoi diagram.
		point_3d size;

		/// Path to the directory to write the result files to.
		///
		/// Must exist.
		///
		/// Default: current directory.
		std::string out_directory;

		/// Amount of threads to use for generation.
		///
		/// Default: however many threads were detected on the system.
		///
		/// Not zero.
		std::size_t jobs;

		/// The colours to use.
		///
		/// If `nullopt` – use default set.
		///
		/// If `std::size_t` – amount to generate (first use default ones, then random gen).
		///
		/// If `std::string` – file with CSS3 colours, one per line.
		nonstd::optional<nonstd::variant<std::size_t, std::string>> colours;

		/// The colours to use.
		///
		/// If `nullopt` – generate `colours.size()` points.
		///
		/// If `std::size_t` – generate `min(points, colours.size())` points.
		///
		/// If `std::string` – file with points in `util::parse_point()` format, one per line;
		///                    only up to `min(points.size(), colours.size())` will be used.
		nonstd::optional<nonstd::variant<std::size_t, std::string>> points;


		/// Attempt to parse command-line arguments.
		///
		/// On success, returns `{parsed_opts, 0, whatever}`.
		///
		/// On error, returns `{_invalid_, exit code != 0, error message}`.
		static nonstd::variant<options, option_err> parse(int argc, const char * const * argv);
	};

	bool operator==(const options & lhs, const options & rhs);
	bool operator!=(const options & lhs, const options & rhs);
}
