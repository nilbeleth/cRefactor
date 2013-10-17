# Project:  C/C++ refactoring library
# Author:   Matej Odalos <xodalo00@stud.fit.vutbr.cz>
# Date:     17.10.2013
#
# Usage:
#


SRC = .
BUILD = ./obj
SOURCES = main.cpp

OBJECTS = $(patsubst %,$(BUILD)/%,$(SOURCES:.cpp=.o))
CXX = g++
CPPFLAGS = 						# preprocessor flags
CXXFLAGS = -std=c++98 -Wall -Wextra -pedantic -W -g	# compile flags
LDFLAGS =						# linker flags

LIBS = 

PROG = librefactor



.PHONY: all pre-build build static shared install pack clean purge doxygen

#
# Object targets
#

all: build

pre-build:
	mkdir -p $(BUILD)


build: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(PROG) $(LIBS)


$(BUILD)/%.o: $(SRC)/%.cpp
	make pre-build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<


static: $(OBJECTS)
	ar rcs $(PROG).a $^

shared: $(OBJECTS)
	$(CXX) -shared -Wl,-soname,$(PROG).so.1 -o $(PROG).so.1.0.1 $^


install:
	@echo "Not implemeted."


pack:
	@echo "Not Implemented."

clean:
	rm -rf $(BUILD)
	rm -f doxygen.log
	rm -rf ./doc/html

purge:
	@make clean
	rm -rf $(PROG) $(PROG).a $(PROG).so*

doxygen:
	doxygen ./doc/doxygen.cfg
