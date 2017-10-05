cmake_minimum_required(VERSION 3.2)

PROJECT(clang-tutorial CXX)

# Will find /usr/local/lib/cmake/clang/ClangConfig.cmake
#
# CAUTION: case-sensitive. If you use "CLANG", cmake will try to find
# CLANGConfig.cmake and fail.
find_package(Clang REQUIRED CONFIG)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti")

add_executable(tutorial6 tutorial6.cpp)

# Link against clang libraries
target_link_libraries(tutorial6 findAllSymbols)
# findAllSymbols is defined in ClangTargets.cmake and depend on llvm
# libraries.

return()




















SET(LLVMCONFIG "/usr/local/bin/llvm-config")
SET(RTTIFLAG "-fno-rtti")

# read the output of "llvm-config option" to output-var
function(read_llvm_config option output-var)
  execute_process(COMMAND ${LLVMCONFIG} ${option} OUTPUT_VARIABLE ${output-var})
  STRING(STRIP ${${output-var}} ${output-var})
  SET(${output-var} ${${output-var}} PARENT_SCOPE)
endfunction()

read_llvm_config("--src-root" LLVM-SRC-ROOT)
read_llvm_config("--obj-root" LLVM-OBJ-ROOT)
read_llvm_config("--cxxflags" LLVM-CXXFLAGS)
# read_llvm_config("--ldflags" LLVM-LDFLAGS)

include_directories(${LLVM-SRC-ROOT}/tools/clang/include)
include_directories(${LLVM-OBJ-ROOT}/tools/clang/include)
include_directories(/usr/local/include/llvm/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RTTIFLAG} ${LLVM-CXXFLAGS}")

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
  read_llvm_config(${option} lib-string)
  link_options_to_list(${lib-string} "-l" lib-list)
  set(${output-var} ${lib-list} PARENT_SCOPE)
endfunction()

read_llvm_config_libs("--libs" LLVM-LIBS)
read_llvm_config_libs("--system-libs" LLVM-SYSTEM-LIBS)
add_library(llvm_library_collection)
set_target_properties(llvm_library_collection PROPERTIES LINKER_LANGUAGE CXX)

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

target_link_libraries(llvm_library_collection
  ${CLANG-DEFAULT-LIBRARIES}
  ${LLVM-LIBS}
  ${LLVM-SYSTEM-LIBS}
  curses)

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
  get_filename_component(executable ${source} NAME_WE)
  add_executable(${executable} ${source})
  target_link_libraries(${executable} llvm_library_collection)
endforeach()
