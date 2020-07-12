#!/bin/bash

cd ~/cpp/my_dt
cp -rf /mnt/e/snort_plugin/my_dt/Tread_file.* .
cp -rf /mnt/e/snort_plugin/my_dt/Tdataframe.* .
cp -rf /mnt/e/snort_plugin/my_dt/Tdec_tree.* .
cp -rf /mnt/e/snort_plugin/my_dt/main.cc .
make
clear
./my_dt 5 data/kddcup.names data/train_full_corrected.csv data/test_full_correctd.csv data/dc_full_tree_dp5.csv 