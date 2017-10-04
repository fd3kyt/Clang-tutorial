cmake_minimum_required(VERSION 3.2)

SET(ENV{CXX} "/usr/local/bin/clang++")
SET(LLVMCONFIG "/usr/local/bin/llvm-config")
SET(RTTIFLAG "-fno-rtti")

# read the output of "llvm-config --<option>" to variable "LLVM-OPTION"
macro(read_llvm_config option)
  STRING(REGEX REPLACE "^-+" "" output-var ${option})
  STRING(TOUPPER ${output-var} output-var)
  STRING(CONCAT output-var "LLVM-" ${output-var})
  execute_process(COMMAND ${LLVMCONFIG} ${option} OUTPUT_VARIABLE ${output-var})
endmacro()
read_llvm_config("--src-root")
message(${LLVM-SRC-ROOT})


return()


# CXXFLAGS := -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) $(RTTIFLAG) -I /usr/local/include/llvm/ --std=c++11
# LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags)

# SOURCES = tutorial1.cpp \
#     tutorial2.cpp \
#     tutorial3.cpp \
#     tutorial4.cpp \
#     tutorial6.cpp \
#     CItutorial1.cpp \
#     CItutorial2.cpp \
#     CItutorial3.cpp \
#     CItutorial4.cpp \
#     CItutorial6.cpp \
#     CIBasicRecursiveASTVisitor.cpp \
#     CIrewriter.cpp \
#     ToolingTutorial.cpp \
#     CommentHandling.cpp

# OBJECTS = $(SOURCES:.cpp=.o)
# EXES = $(OBJECTS:.o=)
# CLANGLIBS = \
# 				-lclangTooling\
# 				-lclangFrontendTool\
# 				-lclangFrontend\
# 				-lclangDriver\
# 				-lclangSerialization\
# 				-lclangCodeGen\
# 				-lclangParse\
# 				-lclangSema\
# 				-lclangStaticAnalyzerFrontend\
# 				-lclangStaticAnalyzerCheckers\
# 				-lclangStaticAnalyzerCore\
# 				-lclangAnalysis\
# 				-lclangARCMigrate\
# 				-lclangRewrite\
# 				-lclangRewriteFrontend\
# 				-lclangEdit\
# 				-lclangAST\
# 				-lclangLex\
# 				-lclangBasic\
# 				$(shell $(LLVMCONFIG) --libs)\
# 				$(shell $(LLVMCONFIG) --system-libs)\
#                 -lcurses
# # "llvm-config --libs" will get a lot.
# # "llvm-config --libs native/engine" will fail

# all: $(OBJECTS) $(EXES)

# %: %.o
# 	$(CXX) -o $@ $< $(CLANGLIBS) $(LLVMLDFLAGS)

# clean:
# 	-rm -f $(EXES) $(OBJECTS) *~
