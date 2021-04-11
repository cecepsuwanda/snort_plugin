#!/bin/bash

mkdir model/model_100_10_svm_org
mkdir model/model_100_10_svm_org/test
mkdir model/model_100_10_svm_org/train
mkdir model/model_100_10_svm_org/model1
mkdir model/model_100_10_svm_org/model2
mkdir model/model_100_10_svm_org/model3
mkdir model/model_100_10_svm_org/model4

rm -rf model/model_100_10_svm_org/hasil.txt
rm -rf model/model_100_10_svm_org/dtsvm_model.csv
rm -rf model/model_100_10_svm_org/all_metrik.csv
rm -rf model/model_100_10_svm_org/dt_metrik.csv
rm -rf model/model_100_10_svm_org/svm_metrik.csv
rm -rf model/model_100_10_svm_org/dtsvm_metrik.csv


./my_dtsvm 0 1 0 0 100 10 0.0001 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model1 0 0 0 0.0 | tee model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 10 0.0001 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model1  0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 10 0.0001 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model/model_100_10_svm_org model1 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 10 0.0001 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model/model_100_10_svm_org model1 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 10 0.0001 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model/model_100_10_svm_org model1 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 10 0.0003 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model2 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 10 0.0003 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model2  0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 10 0.0003 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model/model_100_10_svm_org model2 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 10 0.0003 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model/model_100_10_svm_org model2 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 10 0.0003 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model/model_100_10_svm_org model2 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 10 0.0005 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model3 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 10 0.0005 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model3  0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 10 0.0005 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model/model_100_10_svm_org model3 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 10 0.0005 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model/model_100_10_svm_org model3 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 10 0.0005 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model/model_100_10_svm_org model3 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm 2 1 0 0 100 10 0.001 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model4 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 10 0.001 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model/model_100_10_svm_org model4  0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 10 0.001 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model/model_100_10_svm_org model4 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 10 0.001 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model/model_100_10_svm_org model4 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

./my_dtsvm  1 1 0 0 100 10 0.001 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model/model_100_10_svm_org model4 0 0 0 0.0 | tee -a model/model_100_10_svm_org/hasil.txt
wait

mkdir ~/Dataset/NSL/model_100_10_svm_org

rm -rf ~/Dataset/NSL/model_100_10_svm_org/model*
rm -rf ~/Dataset/NSL/model_100_10_svm_org/test
rm -rf ~/Dataset/NSL/model_100_10_svm_org/train
rm -rf ~/Dataset/NSL/model_100_10_svm_org/dtsvm_model.csv
rm -rf ~/Dataset/NSL/model_100_10_svm_org/hasil.txt
rm -rf ~/Dataset/NSL/model_100_10_svm_org/all_metrik.csv
rm -rf ~/Dataset/NSL/model_100_10_svm_org/dt_metrik.csv
rm -rf ~/Dataset/NSL/model_100_10_svm_org/svm_metrik.csv
rm -rf ~/Dataset/NSL/model_100_10_svm_org/dtsvm_metrik.csv

cp -rf model/model_100_10_svm_org/model* ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/train ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/test ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/dtsvm_model.csv ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/hasil.txt ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/all_metrik.csv ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/dt_metrik.csv ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/svm_metrik.csv ~/Dataset/NSL/model_100_10_svm_org
cp -rf model/model_100_10_svm_org/dtsvm_metrik.csv ~/Dataset/NSL/model_100_10_svm_org

