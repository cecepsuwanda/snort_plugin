#!/bin/bash

mkdir model/model_100_2_svm_42attr_credal1
mkdir model/model_100_2_svm_42attr_credal1/test
mkdir model/model_100_2_svm_42attr_credal1/train
mkdir model/model_100_2_svm_42attr_credal1/model1
mkdir model/model_100_2_svm_42attr_credal1/model2
mkdir model/model_100_2_svm_42attr_credal1/model3
mkdir model/model_100_2_svm_42attr_credal1/model4

rm -rf model/model_100_2_svm_42attr_credal1/hasil.txt
rm -rf model/model_100_2_svm_42attr_credal1/dtsvm_model.csv
rm -rf model/model_100_2_svm_42attr_credal1/all_metrik.csv
rm -rf model/model_100_2_svm_42attr_credal1/dt_metrik.csv
rm -rf model/model_100_2_svm_42attr_credal1/svm_metrik.csv
rm -rf model/model_100_2_svm_42attr_credal1/dtsvm_metrik.csv


./my_dtsvm 0 1 0 0 100 2 0.0001 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model1 0 0 1 1.0 | tee model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model1  0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm_42attr_credal1 model1 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm_42attr_credal1 model1 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm_42attr_credal1 model1 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0003 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model2 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model2  0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm_42attr_credal1 model2 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm_42attr_credal1 model2 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm_42attr_credal1 model2 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0005 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model3 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model3  0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm_42attr_credal1 model3 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm_42attr_credal1 model3 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm_42attr_credal1 model3 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm 2 1 0 0 100 2 0.001 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model4 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_2_svm_42attr_credal1 model4  0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_2_svm_42attr_credal1 model4 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_2_svm_42attr_credal1 model4 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_2_svm_42attr_credal1 model4 0 0 1 1.0 | tee -a model/model_100_2_svm_42attr_credal1/hasil.txt
wait

mkdir ~/Dataset/NSL/model_100_2_svm_42attr_credal1

rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/model*
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/test
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/train
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/dtsvm_model.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/hasil.txt
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/all_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/dt_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/svm_metrik.csv
rm -rf ~/Dataset/NSL/model_100_2_svm_42attr_credal1/dtsvm_metrik.csv

cp -rf model/model_100_2_svm_42attr_credal1/model* ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/train ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/test ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/dtsvm_model.csv ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/hasil.txt ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/all_metrik.csv ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/dt_metrik.csv ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/svm_metrik.csv ~/Dataset/NSL/model_100_2_svm_42attr_credal1
cp -rf model/model_100_2_svm_42attr_credal1/dtsvm_metrik.csv ~/Dataset/NSL/model_100_2_svm_42attr_credal1

