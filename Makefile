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


LDDLLS := $(OS_LD_LIBS) jpeg pb-cpp seed11
LDAR := $(LNCXXAR) $(foreach l,pb-cpp seed11 SFML/lib,-L$(BLDDIR)$(l)) $(foreach dll,$(LDDLLS),-l$(dll))
INCAR := $(foreach l,$(foreach l,optional-lite pb-cpp seed11 SFML TCLAP,$(l)/include),-isystemext/$(l)) $(foreach l,variant-lite,-isystem$(BLDDIR)$(l)/include)
VERAR := $(foreach l,BIG_VORONOI PB_CPP SEED11 TCLAP,-D$(l)_VERSION='$($(l)_VERSION)')
SOURCES := $(sort $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp))
HEADERS := $(sort $(wildcard src/*.hpp src/**/*.hpp src/**/**/*.hpp src/**/**/**/*.hpp))

.PHONY : all clean pb-cpp seed11 sfml variant-lite exe


all : pb-cpp seed11 sfml variant-lite exe

clean :
	rm -rf $(OUTDIR)

exe : pb-cpp sfml variant-lite $(OUTDIR)big-voronoi$(EXE)
pb-cpp : $(BLDDIR)pb-cpp/libpb-cpp$(ARCH)
seed11 : $(BLDDIR)seed11/libseed11$(ARCH)
sfml : $(BLDDIR)SFML/lib/libsfml-graphics-s$(ARCH)
variant-lite : $(BLDDIR)variant-lite/include/nonstd/variant.hpp


$(OUTDIR)big-voronoi$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CXXAR) -o$@ $^ $(PIC) $(LDAR) $(shell grep '<SFML/' $(HEADERS) $(SOURCES) | sed -r 's:.*#include <SFML/(.*).hpp>:-lsfml-\1$(SFML_LINK_SUFF):' | tr '[:upper:]' '[:lower:]' | sort | uniq) $(LDAR)
# '

$(BLDDIR)pb-cpp/libpb-cpp$(ARCH) : ext/pb-cpp/Makefile
	@mkdir -p $(dir $@)
	$(MAKE) -C "$(dir $^)" static OUTDIR="$(abspath $(dir $@))/"

$(BLDDIR)seed11/libseed11$(ARCH) : $(foreach src,seed11_system_agnostic seed11_$(SEED11_SYSTEM_TYPE) deterministic_unsafe_seed_device,$(BLDDIR)seed11/obj/$(src)$(OBJ))
	$(AR) crs $@ $^

$(BLDDIR)SFML/lib/libsfml-graphics-s$(ARCH) : ext/SFML/CMakeLists.txt
	@mkdir -p $(abspath $(dir $@)../build)
	cd $(abspath $(dir $@)..) && $(INCCMAKEAR) $(LNCMAKEAR) $(CMAKE) -DBUILD_SHARED_LIBS=FALSE $(abspath $(dir $^)) -GNinja
	cd $(abspath $(dir $@)..) && $(NINJA) $(notdir $@)

$(BLDDIR)variant-lite/include/nonstd/variant.hpp : ext/variant-lite/include/nonstd/variant.hpp
	@mkdir -p $(dir $@)
	cp $^ $@


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) $(VERAR) -DSFML_STATIC -c -o$@ $^

$(BLDDIR)seed11/obj/%$(OBJ) : ext/seed11/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -isystemext/seed11/include -c -o$@ $^
