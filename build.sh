#!/usr/bin/env bash

mkdir -p build/
cd build || exit 1
CXX=clang++ cmake -GNinja .. && ninja
