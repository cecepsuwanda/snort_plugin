#!/bin/bash

cd ~/cpp/my_dt
cp -rf ~/snort_plugin/my_dt/makefile .
cp -rf ~/snort_plugin/my_dt/src/*.* ./src
make