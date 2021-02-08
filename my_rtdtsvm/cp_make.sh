#!/bin/bash

cd src
cp -rf ~/snort_plugin/my_rtdtsvm/src/*.* .

cd ..
mkdir build-files
cd build-files
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..

cd ..
cmake --build ./build-files --target my_rtdtsvm -- -j 4


cd build-files/src
rm -rf model
mkdir model
cp -rf ~/cpp/my_dtsvm/model .

mkdir output
cd ../..