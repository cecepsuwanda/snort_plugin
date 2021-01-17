#!/bin/bash

cd ~/cpp/my_dtsvm
cp -rf ~/snort_plugin/my_dtsvm/makefile .
cp -rf ~/snort_plugin/my_dtsvm/Tread_file.* .
cp -rf ~/snort_plugin/my_dtsvm/Tdataframe.* .
cp -rf ~/snort_plugin/my_dtsvm/svm.* .
cp -rf ~/snort_plugin/my_dtsvm/Tconf_metrix.* .
cp -rf ~/snort_plugin/my_dtsvm/Tmy_svm.* .
cp -rf ~/snort_plugin/my_dtsvm/Tdec_tree.* .
cp -rf ~/snort_plugin/my_dtsvm/main.cc .
make
wait
clear
wait

# rm data/dtsvm_model*.csv
# rm data/svm_model*.csv
# wait

mkdir model
mkdir model/model1
rm model/model1/dtsvm_model*.csv
rm model/model1/svm_model*.csv
wait

./my_dtsvm  12 30 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model1 1
wait

./my_dtsvm  12 30 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model1 0
wait

./my_dtsvm  12 30 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model1 0
wait

mkdir model/model2
rm model/model2/dtsvm_model*.csv
rm model/model2/svm_model*.csv
wait

./my_dtsvm  12 30 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model2 1
wait

./my_dtsvm  12 30 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model2 0
wait

./my_dtsvm  12 30 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model2 0
wait

mkdir model/model3
rm model/model3/dtsvm_model*.csv
rm model/model3/svm_model*.csv
wait

./my_dtsvm  12 30 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model3 1
wait

./my_dtsvm  12 30 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model3 0
wait

./my_dtsvm  12 30 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model3 0
wait

mkdir model/model4
rm model/model4/dtsvm_model*.csv
rm model/model4/svm_model*.csv
wait

./my_dtsvm  12 30 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model4 1
wait

./my_dtsvm  12 30 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model4 0
wait

./my_dtsvm  12 30 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model4 0
wait


