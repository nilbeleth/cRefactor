# Project:  C/C++ refactoring library
# Author:   Matej Odalos <xodalo00@stud.fit.vutbr.cz>
# Date:     17.10.2013
#
# Usage:
#


SRC = ./src
BUILD = ./obj
SOURCES = resource.cpp \
	  task.cpp renamer.cpp \
	  preprocessorcontext.cpp logger.cpp \
	  main.cpp

CXX = g++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := -fno-rtti
LLVMCONFIG := /usr/bin/llvm-config

CXXFLAGS := -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) -I./src $(RTTIFLAG)
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))


OBJECTS = $(patsubst %,$(BUILD)/%,$(SOURCES:.cpp=.o))
CPPFLAGS = 						# preprocessor flags
$(CXXFLAGS) = -std=c++98 -Wall -Wextra -pedantic -W -g $(CXXFLAGS)
LIBS = \
		-lclangTooling\
		-lclangFrontendTool\
		-lclangFrontend\
		-lclangDriver\
		-lclangSerialization\
		-lclangCodeGen\
		-lclangParse\
		-lclangSema\
		-lclangStaticAnalyzerFrontend\
		-lclangStaticAnalyzerCheckers\
		-lclangStaticAnalyzerCore\
		-lclangAnalysis\
		-lclangARCMigrate\
		-lclangRewriteFrontend\
		-lclangRewriteCore\
		-lclangEdit\
		-lclangAST\
		-lclangLex\
		-lclangBasic\
		$(shell $(LLVMCONFIG) --libs)\
		-lcurses
PROG = librefactor



.PHONY: all pre-build build static shared install pack clean purge doxygen

#
# Object targets
#

all: pre-build build

pre-build:
	-mkdir -p $(BUILD)


build: $(OBJECTS)
	ar rvs $(PROG) $^
	ranlib $(PROG)
#$(CXX) -o app $^ $(LIBS) $(LLVMLDFLAGS)


$(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<


static: $(OBJECTS)
	ar rcs $(PROG).a $^

#ashared: $(OBJECTS)
#	$(CXX) -shared -Wl,-soname,$(PROG).so.1 -o $(PROG).so.1.0.1 $^


install:
	@echo "Not implemeted."


pack:
	@echo "Not Implemented."

clean:
	rm -rf $(BUILD)
	rm -f doxygen.log
	rm -rf ./doc/html
	rm -rf $(PROG)

purge:
	@make clean
	rm -rf $(PROG) $(PROG).a $(PROG).so*

doxygen:
	doxygen ./doc/doxygen.cfg
