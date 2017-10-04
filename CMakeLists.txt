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
read_llvm_config("--obj-root")
read_llvm_config("--cxxflags")
read_llvm_config("--ldflags")

include_directories(${LLVM-SRC-ROOT}/tools/clang/include)
include_directories(${LLVM-OBJ-ROOT}/tools/clang/include)
include_directories(/usr/local/include/llvm/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RTTIFLAG}")

function(link_options_to_list link_option_string prefix output-var)
  set(result)
  separate_arguments(link_options UNIX_COMMAND ${link_option_string})
  foreach(lib ${link_options})
    if(lib MATCHES "^${prefix}")
      string(REGEX REPLACE "^${prefix}" "" lib ${lib})
      list(APPEND result ${lib})
    else()
      message("Option with unexpected prefix, ignoring: ${lib}")
    endif()
  endforeach()
  set(${output-var} ${result} PARENT_SCOPE)
endfunction()

function(read_llvm_config_libs option output-var)

endfunction()

link_options_to_list("-la -lb -Lk -lc" "-l" output)
message("${output}")

return()

SET(SOURCES "tutorial1.cpp
tutorial2.cpp
tutorial3.cpp
tutorial4.cpp
tutorial6.cpp
CItutorial1.cpp
CItutorial2.cpp
CItutorial3.cpp
CItutorial4.cpp
CItutorial6.cpp
CIBasicRecursiveASTVisitor.cpp
CIrewriter.cpp
ToolingTutorial.cpp
CommentHandling.cpp")

foreach(source ${SOURCES})
  string(REGEX REPLACE ".cpp$" "" executable ${source} )
  add_executable(${executable} ${source})
endforeach()


return()



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
