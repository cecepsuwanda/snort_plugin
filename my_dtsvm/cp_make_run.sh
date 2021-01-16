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
# make
# wait
# clear
# wait

rm data/dtsvm_model*.csv
rm data/svm_model*.csv
wait

./my_dtsvm  12 20 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt data/dtsvm_model.csv 1
wait

./my_dtsvm  12 20 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt data/dtsvm_model.csv 0
wait

./my_dtsvm  12 20 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt data/dtsvm_model.csv 0
wait

# rm data/dtsvm_model*.csv
# rm data/svm_model*.csv
# wait

# ./my_dtsvm  12 20 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt data/dtsvm_model.csv 1
# wait

# ./my_dtsvm  12 20 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt data/dtsvm_model.csv 0
# wait

# ./my_dtsvm  12 20 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt data/dtsvm_model.csv 0
# wait

# rm data/dtsvm_model*.csv
# rm data/svm_model*.csv
# wait

# ./my_dtsvm  12 20 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt data/dtsvm_model.csv 1
# wait

# ./my_dtsvm  12 20 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt data/dtsvm_model.csv 0
# wait

# ./my_dtsvm  12 20 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt data/dtsvm_model.csv 0
# wait

# rm data/dtsvm_model*.csv
# rm data/svm_model*.csv
# wait

# ./my_dtsvm  12 20 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt data/dtsvm_model.csv 1
# wait

# ./my_dtsvm  12 20 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt data/dtsvm_model.csv 0
# wait

# ./my_dtsvm  12 20 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt data/dtsvm_model.csv 0
# wait


