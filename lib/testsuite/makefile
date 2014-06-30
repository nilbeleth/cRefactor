CXX := clang++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := #-fno-rtti
LLVMCONFIG := /usr/bin/llvm-config

CXXFLAGS := -std=c++11 -I../inc -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) -g $(RTTIFLAG)
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS)) -L../bin/

SOURCES = tester.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)
LIBS = \
				-lrefactor\
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
				-lclangASTMatchers\
				-lclangBasic\
				$(shell $(LLVMCONFIG) --libs)\

all: build

build: $(OBJECTS)
	$(CXX) -o tester $^ $(LIBS) $(LLVMLDFLAGS)

%: %.o
	$(CXX) -o $@ $< $(CXXFLAGS)

clean:
	-rm -f $(EXES) $(OBJECTS) *~
