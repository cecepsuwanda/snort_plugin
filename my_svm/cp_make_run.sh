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

#rm data/svm_model.csv
#wait


./my_svm  0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model1.csv
wait

./my_svm  0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model2.csv
wait

./my_svm  0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model3.csv
wait

./my_svm  0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model4.csv
wait
