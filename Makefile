# The MIT License (MIT)

# Copyright (c) 2018 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


include configMakefile


LDDLLS := $(OS_LD_LIBS)
LDAR := $(LNCXXAR) $(foreach l,SFML/lib,-L$(BLDDIR)$(l)) $(foreach dll,$(LDDLLS),-l$(dll))
INCAR := $(foreach l,$(foreach l, ,$(l)/include),-isystemext/$(l)) $(foreach l,SFML,-isystem$(BLDDIR)$(l)/include)
VERAR := $(foreach l,BIG_VORONOI,-D$(l)_VERSION='$($(l)_VERSION)')
SOURCES := $(sort $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp))
HEADERS := $(sort $(wildcard src/*.hpp src/**/*.hpp src/**/**/*.hpp src/**/**/**/*.hpp))

.PHONY : all clean sfml exe


all : sfml exe

clean :
	rm -rf $(OUTDIR)

exe : sfml $(OUTDIR)big-voronoi$(EXE)
sfml : $(BLDDIR)SFML/lib/libsfml-system-s$(ARCH)


$(OUTDIR)big-voronoi$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CXXAR) -o$@ $^ $(PIC) $(LDAR) $(shell grep '<SFML/' $(HEADERS) $(SOURCES) | sed -r 's:.*#include <SFML/(.*).hpp>:-lsfml-\1$(SFML_LINK_SUFF):' | tr '[:upper:]' '[:lower:]' | sort | uniq)
# '

$(BLDDIR)SFML/lib/libsfml-system-s$(ARCH) : ext/SFML/CMakeLists.txt
	@mkdir -p $(abspath $(dir $@)../build)
	cd $(abspath $(dir $@)../build) && $(INCCMAKEAR) $(LNCMAKEAR) $(CMAKE) -DBUILD_SHARED_LIBS=FALSE -DCMAKE_INSTALL_PREFIX:PATH="$(abspath $(dir $@)..)" $(abspath $(dir $^)) -GNinja
	cd $(abspath $(dir $@)../build) && $(NINJA) install


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) $(VERAR) -DSFML_STATIC -c -o$@ $^
