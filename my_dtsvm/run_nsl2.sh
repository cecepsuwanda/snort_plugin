#!/bin/bash

mkdir model/model_100_2_svm
mkdir model/model_100_2_svm/test
mkdir model/model_100_2_svm/train
mkdir model/model_100_2_svm/model1
mkdir model/model_100_2_svm/model2
mkdir model/model_100_2_svm/model3
mkdir model/model_100_2_svm/model4

rm -rf model/model_100_2_svm/hasil.txt
rm -rf model/model_100_2_svm/dtsvm_model.csv
rm -rf model/model_100_2_svm/all_metrik.csv
rm -rf model/model_100_2_svm/dt_metrik.csv
rm -rf model/model_100_2_svm/svm_metrik.csv
rm -rf model/model_100_2_svm/dtsvm_metrik.csv


./my_dtsvm 0 1 0 0 100 2 0.0001 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model1 0 0 0 0.0 | tee model/model_100_2_svm/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model1  0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm model1 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm model1 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm model1 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0003 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model2 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model2  0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm model2 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm model2 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm model2 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0005 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model3 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model3  0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm model3 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm model3 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm model3 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm 2 1 0 0 100 2 0.001 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model4 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm model4  0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm model4 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm model4 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm model4 0 0 0 0.0 | tee -a model/model_100_2_svm/hasil.txt
wait

mkdir ~/Dataset/NSL/model_100_2_svm

rm -rf ~/Dataset/NSL/model_100_2_svm/model*
rm -rf ~/Dataset/NSL/model_100_2_svm/test
rm -rf ~/Dataset/NSL/model_100_2_svm/train
rm -rf ~/Dataset/NSL/model_100_2_svm/dtsvm_model.csv
rm -rf ~/Dataset/NSL/model_100_2_svm/hasil.txt
rm -rf ~/Dataset/NSL/model_100_2_svm/all_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm/dt_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm/svm_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm/dtsvm_metrik.csv

cp -rf model/model_100_2_svm/model* ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/train ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/test ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/dtsvm_model.csv ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/hasil.txt ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/all_metrik.csv ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/dt_metrik.csv ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/svm_metrik.csv ~/Dataset/NSL/model_100_2_svm
cp -rf model/model_100_2_svm/dtsvm_metrik.csv ~/Dataset/NSL/model_100_2_svm

