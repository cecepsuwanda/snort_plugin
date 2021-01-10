#!/bin/bash

cd ~/cpp/my_dtsvm
cp -rf /mnt/e/snort_plugin/my_dtsvm/makefile .
cp -rf /mnt/e/snort_plugin/my_dtsvm/Tread_file.* .
cp -rf /mnt/e/snort_plugin/my_dtsvm/Tdataframe.* .
cp -rf /mnt/e/snort_plugin/my_dtsvm/svm.* .
cp -rf /mnt/e/snort_plugin/my_dtsvm/Tconf_metrix.* .
cp -rf /mnt/e/snort_plugin/my_dtsvm/Tmy_svm.* .
cp -rf /mnt/e/snort_plugin/my_dtsvm/Tdec_tree.* .
cp -rf /mnt/e/snort_plugin/my_dtsvm/main.cc .
make
# wait
clear
# wait

rm data/dtsvm_model*.csv
rm data/svm_model*.csv
rm data/save_model*.csv
wait


./my_dtsvm  10 1000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt data/dtsvm_model.csv
wait


