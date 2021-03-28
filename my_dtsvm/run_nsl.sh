#!/bin/bash

mkdir model
mkdir model/test
mkdir model/train
mkdir model/model1
mkdir model/model2
mkdir model/model3
mkdir model/model4

mkdir ~/Dataset/NSL/model_100_30000_svm_30

./my_dtsvm 0 1 0 0 100 30000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model1 0 0 0 | tee ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 30000 0.0001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model1 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 30000 0.0001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model1 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 30000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model1 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 30000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model1 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 30000 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 30000 0.0003 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 30000 0.0003 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 30000 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 30000 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait



./my_dtsvm 2 1 0 0 100 30000 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 30000 0.0005 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 30000 0.0005 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 30000 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 30000 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait



./my_dtsvm 2 1 0 0 100 30000 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 30000 0.001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 30000 0.001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 30000 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 30000 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_30000_svm_30/hasil.txt
wait



rm -rf ~/Dataset/NSL/model_100_30000_svm_30/model*
rm -rf ~/Dataset/NSL/model_100_30000_svm_30/test
rm -rf ~/Dataset/NSL/model_100_30000_svm_30/train
rm -rf ~/Dataset/NSL/model_100_30000_svm_30/dtsvm_model.csv

cp -rf model/model* ~/Dataset/NSL/model_100_30000_svm_30
cp -rf model/train ~/Dataset/NSL/model_100_30000_svm_30
cp -rf model/test ~/Dataset/NSL/model_100_30000_svm_30
cp -rf model/dtsvm_model.csv ~/Dataset/NSL/model_100_30000_svm_30


