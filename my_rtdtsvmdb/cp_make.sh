#!/bin/bash

cd src
cp -rf ~/snort_plugin/my_rtdtsvmdb/src/*.* .

cd ..
mkdir build-files
cd build-files
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..

cd ..
cmake --build ./build-files --target my_rtdtsvmdb -- -j 4
