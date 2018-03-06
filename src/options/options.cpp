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


#include "options.hpp"
#include "existing_dir_constraint.hpp"
#include "output_size_constraint.hpp"
#include "positive_constraint.hpp"
#include <tclap/CmdLine.h>
#include <tclap/SwitchArg.h>
#include <tclap/ValueArg.h>


using namespace std::literals;


nonstd::variant<big_voronoi::options, big_voronoi::option_err> big_voronoi::options::parse(int argc, const char * const * argv) {
	options ret;

	try {
		const auto jobs_s = std::to_string(num_cpus());

		big_voronoi::output_size_constraint size_constraint("DDDxDDDxDDD; D=digit");
		big_voronoi::existing_dir_constraint out_image_constraint("existing directory");
		big_voronoi::positive_constraint jobs_constraint("positive integer");

		TCLAP::CmdLine command_line("big-voronoi -- consider: voronoi diagram, but in 3D and in parallel", ' ', BIG_VORONOI_VERSION);
		TCLAP::ValueArg<std::string> size("s", "size", "Output voronoi resolution. Default: 900x900x900", false, "900x900x900", &size_constraint, command_line);
		TCLAP::ValueArg<std::string> out_dir("o", "out_image", "Directory to write the voronoi diagram to. Default: current directory", false, "",
		                                     &out_image_constraint, command_line);
		TCLAP::ValueArg<std::string> jobs("j", "jobs", "Amount of threads to utilise. Default: " + jobs_s, false, jobs_s, &jobs_constraint, command_line);

		command_line.setExceptionHandling(false);
		command_line.parse(argc, argv);

		ret.size = *parse_size_option(size);

		if(out_dir.getValue().empty())
			ret.out_directory = "";
		else {
			ret.out_directory = out_dir;
			if(ret.out_directory.back() != '/' && ret.out_directory.back() != '\\')
				ret.out_directory.push_back('\\');
		}

		ret.jobs = std::stoull(jobs);
	} catch(const TCLAP::ArgException & e) {
		auto arg_id = e.argId();
		if(arg_id == " ")
			arg_id = "undefined argument";
		return std::make_pair(1, std::string(argv[0]) + ": error: parsing arguments failed (" + e.error() + ") for " + arg_id);
	} catch(const TCLAP::ExitException & e) {
		return std::make_pair(e.getExitStatus() ? e.getExitStatus() : 1, ""s);
	}

	return std::move(ret);
}

bool big_voronoi::operator==(const options & lhs, const options & rhs) {
	return lhs.size == rhs.size && lhs.out_directory == rhs.out_directory;
}

bool big_voronoi::operator!=(const options & lhs, const options & rhs) {
	return !(lhs == rhs);
}
