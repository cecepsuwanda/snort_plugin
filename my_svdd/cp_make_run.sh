#!/bin/bash

cd ~/cpp/my_svdd
cp -rf ~/snort_plugin/my_svdd/makefile .
cp -rf ~/snort_plugin/my_svdd/src/*.* ./src
make

