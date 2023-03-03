#!/bin/bash

cd ~/cpp/my_dtdb
cp -rf ~/snort_plugin/my_dtdb/makefile .
cp -rf ~/snort_plugin//my_dtdb/src/*.* ./src
make