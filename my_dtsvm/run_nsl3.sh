#!/bin/bash

mkdir model/model_100_2_svm_29attr
mkdir model/model_100_2_svm_29attr/test
mkdir model/model_100_2_svm_29attr/train
mkdir model/model_100_2_svm_29attr/model1
mkdir model/model_100_2_svm_29attr/model2
mkdir model/model_100_2_svm_29attr/model3
mkdir model/model_100_2_svm_29attr/model4

rm -rf model/model_100_2_svm_29attr/hasil.txt
rm -rf model/model_100_2_svm_29attr/dtsvm_model.csv
rm -rf model/model_100_2_svm_29attr/all_metrik.csv
rm -rf model/model_100_2_svm_29attr/dt_metrik.csv
rm -rf model/model_100_2_svm_29attr/svm_metrik.csv
rm -rf model/model_100_2_svm_29attr/dtsvm_metrik.csv


./my_dtsvm 0 1 0 0 100 2 0.0001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model1 0 0 0 0.0 | tee model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model1  0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr model1 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr model1 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr model1 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0003 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model2 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model2  0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr model2 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr model2 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr model2 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0005 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model3 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model3  0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr model3 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr model3 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr model3 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm 2 1 0 0 100 2 0.001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model4 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr model4  0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr model4 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr model4 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr model4 0 0 0 0.0 | tee -a model/model_100_2_svm_29attr/hasil.txt
wait

mkdir ~/Dataset/NSL/model_100_2_svm_29attr

rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/model*
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/test
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/train
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/dtsvm_model.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/hasil.txt
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/all_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/dt_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/svm_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr/dtsvm_metrik.csv

cp -rf model/model_100_2_svm_29attr/model* ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/train ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/test ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/dtsvm_model.csv ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/hasil.txt ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/all_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/dt_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/svm_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr
cp -rf model/model_100_2_svm_29attr/dtsvm_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr

