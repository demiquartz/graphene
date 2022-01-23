#!/bin/bash

if [ $# -gt 0 ]; then BuildType=$1; else BuildType="Release"; fi

if type ninja > /dev/null 2>&1; then
    cmake -S src -B build -G "Ninja" -DCMAKE_BUILD_TYPE=$BuildType
    ninja -C build
else
    cmake -S src -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$BuildType
    make -C build
fi
