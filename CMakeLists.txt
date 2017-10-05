cmake_minimum_required(VERSION 3.2)

PROJECT(clang-tutorial CXX)

# Will find /usr/local/lib/cmake/clang/ClangConfig.cmake
#
# CAUTION: case-sensitive. If you use "CLANG", cmake will try to find
# CLANGConfig.cmake and fail.
find_package(Clang REQUIRED CONFIG)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti")

SET(SOURCES
  tutorial1.cpp
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

  # Link against clang libraries
  target_link_libraries(${executable} findAllSymbols)
  # findAllSymbols is defined in ClangTargets.cmake and depend on llvm
  # libraries.
endforeach()
