// The MIT License (MIT)

// Copyright (c) 2018 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include "constraints.hpp"
#include "../util.hpp"
#include <cstring>


#define DEFINE_CONSTRAINT(constraint_name, desc, check_expr)                                                    \
	std::string big_voronoi::constraint_name##_constraint::description() const { return desc; }                   \
                                                                                                                \
	std::string big_voronoi::constraint_name##_constraint::shortID() const { return arg_name; }                   \
                                                                                                                \
	bool big_voronoi::constraint_name##_constraint::check(const std::string & value) const { return check_expr; } \
                                                                                                                \
	big_voronoi::constraint_name##_constraint::constraint_name##_constraint(std::string argname) : arg_name(std::move(argname)) {}


DEFINE_CONSTRAINT(positive, "positive integer", value.find_first_not_of("0123456789") == std::string::npos && std::stoull(value) != 0)
DEFINE_CONSTRAINT(output_size, "D+xD+xD+; D=digit", static_cast<bool>(parse_size_option(value)))
DEFINE_CONSTRAINT(existing_dir, "existing directory", directory_exists(value))
DEFINE_CONSTRAINT(positive_or_existing_file, "positive integer or path to existing file", std::atoll(value.c_str()) > 0 || file_exists(value))


#undef DEFINE_CONSTRAINT
