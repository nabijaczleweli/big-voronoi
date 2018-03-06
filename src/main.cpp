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


#include "options/options.hpp"
#include <iostream>


int main(int argc, const char ** argv) {
	const auto opts_r = big_voronoi::options::parse(argc, argv);
	if(const auto error_val = nonstd::get_if<big_voronoi::option_err>(&opts_r)) {
		std::cerr << error_val->second << '\n';
		return error_val->first;
	}
	const auto opts = std::move(nonstd::get<big_voronoi::options>(opts_r));

	std::cout << "Generating a " << std::get<0>(opts.size) << 'x' << std::get<1>(opts.size) << 'x' << std::get<2>(opts.size) << " voronoi diagram on "
	          << opts.jobs << " threads to " << (opts.out_directory.empty() ? "./" : opts.out_directory.c_str()) << '\n';
}
