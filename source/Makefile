#
# Copyright (c) 2016-present, Facebook, Inc.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.
#

CXX = c++
CXXFLAGS = -pthread -std=c++0x
OBJDIR=build
OBJS = $(addprefix $(OBJDIR)/, \
	    $(addprefix src/, args.o autotune.o densematrix.o dictionary.o fasttext.o loss.o matrix.o meter.o model.o productquantizer.o quantmatrix.o utils.o vector.o) \
	    $(addprefix server/, \
		serverfasttext.o http_server.o urldecode.o strmap.o fasttext_http_modules.o \
		$(addprefix encoding/, encoding.o ascii.o utf8.o utf16.o) \
		$(addprefix modules/analogies/, analogies.o ) \
		$(addprefix modules/wordvectors/, wordvectors.o ) \
		$(addprefix modules/similarity/, similarity.o ) \
		$(addprefix modules/similarity/, negsimilarity.o ) \
		$(addprefix modules/classifier/, classifier.o ) \
	    ) \
	)
INCLUDES = -I.
SHELL=/bin/bash

opt: CXXFLAGS += -O3 -funroll-loops
opt: fasttext

debug: CXXFLAGS += -g -O0 -fno-inline
debug: fasttext

script:
	$(shell mkdir -p build/{src,server/{encoding,modules/{analogies,wordvectors,similarity,classifier,}}})

$(OBJDIR)/src/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/server/%.o: server/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/server/%.o: server/%.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/server/encoding/%.o: server/encoding/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/server/modules/analogies/%.o: server/modules/analogies/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/server/modules/analogies/%.o: server/modules/wordvectors/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/server/modules/similarity/%.o: server/modules/similarity/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/server/modules/classifier/%.o: server/modules/classifier/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

fasttext: script $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) server/servermain.cc -o fasttext

fasttext-static: script $(OBJS)
	$(CXX) -static $(CXXFLAGS) $(OBJS) server/servermain.cc -o fasttext-static

clean:
	$(shell rm -rf build/* fasttext fasttext-static)
