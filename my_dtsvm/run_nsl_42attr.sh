#!/bin/bash

mkdir model/model_100_128_svm_42attr
mkdir model/model_100_128_svm_42attr/test
mkdir model/model_100_128_svm_42attr/train
mkdir model/model_100_128_svm_42attr/model1
mkdir model/model_100_128_svm_42attr/model2
mkdir model/model_100_128_svm_42attr/model3
mkdir model/model_100_128_svm_42attr/model4

rm -rf model/model_100_128_svm_42attr/hasil.txt
rm -rf model/model_100_128_svm_42attr/dtsvm_model.csv
rm -rf model/model_100_128_svm_42attr/all_metrik.csv
rm -rf model/model_100_128_svm_42attr/dt_metrik.csv
rm -rf model/model_100_128_svm_42attr/svm_metrik.csv
rm -rf model/model_100_128_svm_42attr/dtsvm_metrik.csv


./my_dtsvm 0 1 0 0 100 128 0.0001 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model1 0 0 0 0.0 | tee model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 128 0.0001 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model1  0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 128 0.0001 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_128_svm_42attr model1 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 128 0.0001 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_128_svm_42attr model1 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 128 0.0001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_128_svm_42attr model1 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 128 0.0003 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model2 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 128 0.0003 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model2  0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 128 0.0003 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_128_svm_42attr model2 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 128 0.0003 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_128_svm_42attr model2 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 128 0.0003 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_128_svm_42attr model2 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 128 0.0005 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model3 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 128 0.0005 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model3  0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 128 0.0005 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_128_svm_42attr model3 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 128 0.0005 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_128_svm_42attr model3 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 128 0.0005 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_128_svm_42attr model3 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm 2 1 0 0 100 128 0.001 0.01  data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model4 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 128 0.001 0.01 data/kddcup.names data/NSLTree_train.txt model/model_100_128_svm_42attr model4  0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 128 0.001 0.01 data/kddcup.names data/NSLTree_test.txt model/model_100_128_svm_42attr model4 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 128 0.001 0.01  data/kddcup.names data/NSLTree_test1.txt model/model_100_128_svm_42attr model4 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 128 0.001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/model_100_128_svm_42attr model4 0 0 0 0.0 | tee -a model/model_100_128_svm_42attr/hasil.txt
wait

mkdir ~/Dataset/NSL/model_100_128_svm_42attr

rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/model*
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/test
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/train
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/dtsvm_model.csv
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/hasil.txt
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/all_metrik.csv
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/dt_metrik.csv
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/svm_metrik.csv
rm -rf ~/Dataset/NSL/model_100_128_svm_42attr/dtsvm_metrik.csv

cp -rf model/model_100_128_svm_42attr/model* ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/train ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/test ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/dtsvm_model.csv ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/hasil.txt ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/all_metrik.csv ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/dt_metrik.csv ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/svm_metrik.csv ~/Dataset/NSL/model_100_128_svm_42attr
cp -rf model/model_100_128_svm_42attr/dtsvm_metrik.csv ~/Dataset/NSL/model_100_128_svm_42attr

