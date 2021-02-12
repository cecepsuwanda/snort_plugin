#!/bin/bash

cd ~/cpp/my_dtsvm
cp -rf ~/snort_plugin/my_dtsvm/makefile .
cp -rf ~/snort_plugin/my_dtsvm/Tread_file.* .
cp -rf ~/snort_plugin/my_dtsvm/Tdataframe.* .
cp -rf ~/snort_plugin/my_dtsvm/svm.* .
cp -rf ~/snort_plugin/my_dtsvm/Tconf_metrix.* .
cp -rf ~/snort_plugin/my_dtsvm/Tmy_svm.* .
cp -rf ~/snort_plugin/my_dtsvm/Tdec_tree.* .
cp -rf ~/snort_plugin/my_dtsvm/Tmy_dttype.* .
cp -rf ~/snort_plugin/my_dtsvm/Tlabel_stat.* .
cp -rf ~/snort_plugin/my_dtsvm/Tbelow_above.* .
cp -rf ~/snort_plugin/my_dtsvm/Tbase_dataframe.* .
cp -rf ~/snort_plugin/my_dtsvm/main.cc .
make



