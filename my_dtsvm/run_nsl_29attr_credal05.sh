#!/bin/bash

mkdir model/model_100_2_svm_29attr_credal05
mkdir model/model_100_2_svm_29attr_credal05/test
mkdir model/model_100_2_svm_29attr_credal05/train
mkdir model/model_100_2_svm_29attr_credal05/model1
mkdir model/model_100_2_svm_29attr_credal05/model2
mkdir model/model_100_2_svm_29attr_credal05/model3
mkdir model/model_100_2_svm_29attr_credal05/model4

rm -rf model/model_100_2_svm_29attr_credal05/hasil.txt
rm -rf model/model_100_2_svm_29attr_credal05/dtsvm_model.csv
rm -rf model/model_100_2_svm_29attr_credal05/all_metrik.csv
rm -rf model/model_100_2_svm_29attr_credal05/dt_metrik.csv
rm -rf model/model_100_2_svm_29attr_credal05/svm_metrik.csv
rm -rf model/model_100_2_svm_29attr_credal05/dtsvm_metrik.csv


./my_dtsvm 0 1 0 0 100 2 0.0001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model1 0 0 1 0.5 | tee model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model1  0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr_credal05 model1 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr_credal05 model1 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr_credal05 model1 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0003 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model2 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model2  0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr_credal05 model2 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr_credal05 model2 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr_credal05 model2 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0005 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model3 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model3  0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr_credal05 model3 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr_credal05 model3 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr_credal05 model3 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm 2 1 0 0 100 2 0.001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model4 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_train.txt model/model_100_2_svm_29attr_credal05 model4  0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup_29attr.names data/NSLTree_29attr_test.txt model/model_100_2_svm_29attr_credal05 model4 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_test1.txt model/model_100_2_svm_29attr_credal05 model4 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup_29attr.names data/NSLTree_29attr_unknown.txt model/model_100_2_svm_29attr_credal05 model4 0 0 1 0.5 | tee -a model/model_100_2_svm_29attr_credal05/hasil.txt
wait

mkdir ~/Dataset/NSL/model_100_2_svm_29attr_credal05

rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/model*
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/test
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/train
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/dtsvm_model.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/hasil.txt
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/all_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/dt_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/svm_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_29attr_credal05/dtsvm_metrik.csv

cp -rf model/model_100_2_svm_29attr_credal05/model* ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/train ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/test ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/dtsvm_model.csv ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/hasil.txt ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/all_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/dt_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/svm_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr_credal05
cp -rf model/model_100_2_svm_29attr_credal05/dtsvm_metrik.csv ~/Dataset/NSL/model_100_2_svm_29attr_credal05
