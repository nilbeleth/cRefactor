# Project:  C/C++ refactoring library
# Author:   Matej Odalos <xodalo00@stud.fit.vutbr.cz>
# Date:     17.10.2013
#
# Usage:
#


SRC = ./src
BUILD = ./obj
SOURCES = location.cpp resource.cpp replacement.cpp \
	  task.cpp renamer.cpp reformatter.cpp extractor.cpp \
	  logger.cpp utils.cpp renamingstrategy.cpp renamerbyname.cpp
HEADERS = location.h resource.h replacement.h \
	  task.h renamer.h reformatter.h extractor.h \
	  logger.h defines.h


CXX = clang++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := #-fno-rtti
LLVMCONFIG := /usr/bin/llvm-config

CXXFLAGS := -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) -I./src $(RTTIFLAG) -g
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))


OBJECTS = $(patsubst %,$(BUILD)/%,$(SOURCES:.cpp=.o))
CPPFLAGS = 						# preprocessor flags
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -W -g $(CXXFLAGS)
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
		-lclangASTMatchers\
		-lclangLex\
		-lclangBasic\
		$(shell $(LLVMCONFIG) --libs)\
		-lcurses
PROG = librefactor



.PHONY: all pre-build build install pack clean purge doxygen

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


install:
	mkdir -p bin/ inc/
	cp $(addprefix $(SRC)/, $(HEADERS)) ./inc/
	mv librefactor ./bin/librefactor.a


pack:
	@echo "Not Implemented."

clean:
	rm -rf $(BUILD)
	rm -rf $(PROG)

uninstall:
	rm -rf ./inc/* ./bin/*

purge:
	@make clean
	@make uninstall
	rm -rf doxygen.log
	rm -rf ./doc/html

doxygen:
	doxygen ./doc/doxygen.cfg
