#!/bin/bash


./my_dtsvm  26 1000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model1 1 1 0 0
wait

./my_dtsvm  26 1000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model1 0 1 0 0
wait

./my_dtsvm  26 1000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model1 0 1 0 0
wait


./my_dtsvm  26 1000 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model2 1 1 0 0
wait

./my_dtsvm  26 1000 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model2 0 1 0 0
wait

./my_dtsvm  26 1000 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model2 0 1 0 0
wait


./my_dtsvm  26 1000 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model3 1 1 0 0
wait

./my_dtsvm  26 1000 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model3 0 1 0 0
wait

./my_dtsvm  26 1000 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model3 0 1 0 0
wait


./my_dtsvm  26 1000 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model4 1 1 0 0
wait

./my_dtsvm  26 1000 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model4 0 1 0 0
wai1

./my_dtsvm  26 1000 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model4 0 1 0 0
wait

cp -rf model/model* ~/Dataset/NSL/model_26_1000_svm


