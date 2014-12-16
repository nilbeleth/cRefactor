CXX := clang++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := #-fno-rtti
LLVMCONFIG := /usr/bin/llvm-config

CXXFLAGS := -std=c++11 -I./lib -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) -g $(RTTIFLAG)
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --system-libs --libs $(LLVMCOMPONENTS)) -L./lib/bin/

SOURCES = main.cpp

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
				-lclangRewrite\
				-lclangRewriteFrontend\
				-lclangApplyReplacements\
				-lclangEdit\
				-lclangAST\
				-lclangLex\
				-lclangASTMatchers\
				-lclangBasic\
				-lclangIndex\
				$(shell $(LLVMCONFIG) --libs)\

all: build

build: $(OBJECTS)
	$(CXX) -o app $^ $(LIBS) $(LLVMLDFLAGS)

%: %.o
	$(CXX) -o $@ $< $(CXXFLAGS)

clean:
	-rm -f $(EXES) $(OBJECTS) *~
	rm -f compile_commands.json
