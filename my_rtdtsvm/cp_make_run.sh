#!/bin/bash

cd src
cp -rf ~/snort_plugin/my_rtdtsvm/src/*.* .

cd ..
cd build-files
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..

cd ..
cmake --build ./build-files --target my_rtdtsvm -- -j 4