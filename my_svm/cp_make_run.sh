#!/bin/bash

cd ~/cpp/my_svm
cp -rf /mnt/e/snort_plugin/my_svm/makefile .
cp -rf /mnt/e/snort_plugin/my_svm/Tread_file.* .
cp -rf /mnt/e/snort_plugin/my_svm/Tdataframe.* .
cp -rf /mnt/e/snort_plugin/my_svm/svm.* .
cp -rf /mnt/e/snort_plugin/my_svm/Tconf_metrix.* .
cp -rf /mnt/e/snort_plugin/my_svm/Tmy_svm.* .
cp -rf /mnt/e/snort_plugin/my_svm/main.cc .
make
# wait
clear
# wait

rm data/svm_model.csv
wait


./my_svm data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model.csv
wait
