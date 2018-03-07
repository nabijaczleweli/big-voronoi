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


#include "util.hpp"
#include <regex>
#include <sys/stat.h>
#include <sys/types.h>


static std::regex parse_size_regex{"([[:digit:]]+)x([[:digit:]]+)x([[:digit:]]+)"};


nonstd::optional<big_voronoi::point_3d> big_voronoi::parse_size_option(const char * opt) {
	std::cmatch match;
	if(std::regex_match(opt, match, parse_size_regex))
		return std::make_tuple(std::stoull(match[1].str()), std::stoull(match[2].str()), std::stoull(match[3].str()));
	else
		return nonstd::nullopt;
}

nonstd::optional<big_voronoi::point_3d> big_voronoi::parse_size_option(const std::string & opt) {
	return parse_size_option(opt.c_str());
}

bool big_voronoi::directory_exists(const char * path) {
	struct stat info;
	return stat(path, &info) == 0 && info.st_mode & S_IFDIR;
}

bool big_voronoi::directory_exists(const std::string & path) {
	return directory_exists(path.c_str());
}

std::string big_voronoi::separated_number(std::size_t num, char separator) {
	auto ss = std::to_string(num);
	std::reverse(std::begin(ss), std::end(ss));
	for(std::size_t i = 3; i < ss.size(); i += 4)
		ss.insert(i, 1, separator);
	std::reverse(std::begin(ss), std::end(ss));
	return ss;
}


#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


std::size_t big_voronoi::num_cpus() {
	SYSTEM_INFO sysinfo{};
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}

#else

#include <unistd.h>


std::size_t big_voronoi::num_cpus() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}

#endif
