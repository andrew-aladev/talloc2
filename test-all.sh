#!/bin/bash

CC="gcc-3.4.6" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
CC="gcc-4.8.2" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
CC="tcc" ../combinations.sh -DTRALLOC_SHARED=0 -DCMAKE_BUILD_TYPE="RELEASE"
NO_TESTS=1 ../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/arduino.cmake -DTRALLOC_SHARED=0 -DCMAKE_BUILD_TYPE="RELEASE"
CC="clang" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
CC="ccc-analyzer" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"