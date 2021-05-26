#!/bin/bash

cd ~/cpp/my_dtsvm
cp -rf ~/snort_plugin/my_dtsvm/makefile .
cp -rf ~/snort_plugin/my_dtsvm/src/*.* ./src
make



