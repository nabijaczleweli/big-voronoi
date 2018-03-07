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


#include <cstdint>
#include <nonstd/optional.hpp>
#include <string>
#include <tuple>


namespace big_voronoi {
	using point_3d = std::tuple<std::size_t, std::size_t, std::size_t>;


	/// Parse a string on the format `"D+xD+xD+"` where `D` is a digit.
	///
	/// Returns `nullopt` if the parsing failed.
	nonstd::optional<point_3d> parse_size_option(const char * opt);
	nonstd::optional<point_3d> parse_size_option(const std::string & opt);

	/// Check if the file specified by the path exists and is a directory.
	bool directory_exists(const char * path);
	bool directory_exists(const std::string & path);

	/// Get the amount of logical processors.
	std::size_t num_cpus();

	/// Separate number with the specified separator.
	///
	/// Examples:
	///
	/// ```
	/// assert_eq!(separated_number(1234567890), "1'234'567'890");
	/// ```
	std::string separated_number(std::size_t num, char separator = '\'');
}
