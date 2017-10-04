cmake_minimum_required(VERSION 3.2)

PROJECT(clang-tutorial C CXX)

SET(ENV{CXX} "/usr/local/bin/clang++")
SET(LLVMCONFIG "/usr/local/bin/llvm-config")
SET(RTTIFLAG "-fno-rtti")

# read the output of "llvm-config --<option>" to variable "LLVM-OPTION"
function(read_llvm_config option)
  STRING(REGEX REPLACE "^-+" "" output-var ${option})
  STRING(TOUPPER ${output-var} output-var)
  STRING(CONCAT output-var "LLVM-" ${output-var})
  execute_process(COMMAND ${LLVMCONFIG} ${option} OUTPUT_VARIABLE ${output-var})
  STRING(STRIP ${${output-var}} ${output-var})
  SET(${output-var} ${${output-var}} PARENT_SCOPE)
endfunction()
read_llvm_config("--src-root")
read_llvm_config("--obj-root")
read_llvm_config("--cxxflags")
read_llvm_config("--ldflags")

include_directories(${LLVM-SRC-ROOT}/tools/clang/include)
include_directories(${LLVM-OBJ-ROOT}/tools/clang/include)
include_directories(/usr/local/include/llvm/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RTTIFLAG} ${LLVM-CXXFLAGS}")
message(${LLVM-CXXFLAGS})

function(link_options_to_list link_option_string prefix output-var)
  set(result)
  separate_arguments(link_options UNIX_COMMAND ${link_option_string})
  foreach(lib ${link_options})
    if(lib MATCHES "^${prefix}")
      string(REGEX REPLACE "^${prefix}" "" lib ${lib})
      list(APPEND result ${lib})
    else()
      message(WARNING "Option with unexpected prefix, ignoring: ${lib}")
    endif()
  endforeach()
  set(${output-var} ${result} PARENT_SCOPE)
endfunction()

function(read_llvm_config_libs option output-var)
  execute_process(COMMAND ${LLVMCONFIG} ${option} OUTPUT_VARIABLE lib-string)
  link_options_to_list(${lib-string} "-l" lib-list)
  set(${output-var} ${lib-list} PARENT_SCOPE)
endfunction()

read_llvm_config_libs("--libs" LLVM-LIBS)
read_llvm_config_libs("--system-libs" LLVM-SYSTEM-LIBS)
add_library(clang_library)
# message("${LLVM-SYSTEM-LIBS}")
set_target_properties(clang_library PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(clang_library ${LLVM-LIBS} ${LLVM-SYSTEM-LIBS})

SET(CLANG-DEFAULT-LIBRARIES
  clangTooling
  clangFrontendTool
  clangFrontend
  clangDriver
  clangSerialization
  clangCodeGen
  clangParse
  clangSema
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
  clangAnalysis
  clangARCMigrate
  clangRewrite
  clangRewriteFrontend
  clangEdit
  clangAST
  clangLex
  clangBasic
  )

SET(SOURCES tutorial1.cpp
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
  CommentHandling.cpp)

foreach(source ${SOURCES})
  string(REGEX REPLACE ".cpp$" "" executable ${source} )
  add_executable(${executable} ${source})
  target_link_libraries(${executable}
    ${CLANG-DEFAULT-LIBRARIES}
    ${LLVM-LIBS}
    ${LLVM-SYSTEM-LIBS}
    curses)
endforeach()
