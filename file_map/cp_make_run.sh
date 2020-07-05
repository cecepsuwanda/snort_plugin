#!/bin/bash

cd ~/cpp
cp -rf /mnt/e/snort_plugin/file_map .
cd file_map
make
./file_map