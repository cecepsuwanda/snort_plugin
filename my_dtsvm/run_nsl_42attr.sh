#!/bin/bash

PATH_MODEL='model_100_600_svm_42attr' 
DEPTH=100
MIN_SAMP=600
USE_CREDAL=0
CREDAL_S=0.0
LIMIT=0
F=0
N=1

mkdir model/$PATH_MODEL
mkdir model/$PATH_MODEL/test
mkdir model/$PATH_MODEL/train
mkdir model/$PATH_MODEL/model1
mkdir model/$PATH_MODEL/model2
mkdir model/$PATH_MODEL/model3
mkdir model/$PATH_MODEL/model4

rm -rf model/$PATH_MODEL/hasil.txt
rm -rf model/$PATH_MODEL/dtsvm_model.csv
rm -rf model/$PATH_MODEL/all_metrik.csv
rm -rf model/$PATH_MODEL/dt_metrik.csv
rm -rf model/$PATH_MODEL/svm_metrik.csv
rm -rf model/$PATH_MODEL/dtsvm_metrik.csv


./my_dtsvm 0 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model1 1 0 $USE_CREDAL $CREDAL_S $LIMIT | tee model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model1  0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 data/kddcup.names data/NSLTree_test.txt model/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  data/kddcup.names data/NSLTree_test1.txt model/$PATH_MODEL model1 0 1 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model2  0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 data/kddcup.names data/NSLTree_test.txt model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt 
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  data/kddcup.names data/NSLTree_test1.txt model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  data/kddcup.names data/NSLTree_unknown.txt model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model3  0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 data/kddcup.names data/NSLTree_test.txt model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  data/kddcup.names data/NSLTree_test1.txt model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  data/kddcup.names data/NSLTree_unknown.txt model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model4  0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 data/kddcup.names data/NSLTree_test.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  data/kddcup.names data/NSLTree_test1.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  data/kddcup.names data/NSLTree_unknown.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.003 0.1  data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.1 data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model4  0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.1 data/kddcup.names data/NSLTree_test.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.1  data/kddcup.names data/NSLTree_test1.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.1  data/kddcup.names data/NSLTree_unknown.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.004 0.5  data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.5 data/kddcup.names data/NSLTree_train.txt model/$PATH_MODEL model4  0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.5 data/kddcup.names data/NSLTree_test.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.5  data/kddcup.names data/NSLTree_test1.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.5  data/kddcup.names data/NSLTree_unknown.txt model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT | tee -a model/$PATH_MODEL/hasil.txt
wait

mkdir ~/Dataset/NSL/$PATH_MODEL

rm -rf ~/Dataset/NSL/$PATH_MODEL/model*
rm -rf ~/Dataset/NSL/$PATH_MODEL/test
rm -rf ~/Dataset/NSL/$PATH_MODEL/train
rm -rf ~/Dataset/NSL/$PATH_MODEL/dtsvm_model.csv
rm -rf ~/Dataset/NSL/$PATH_MODEL/hasil.txt
rm -rf ~/Dataset/NSL/$PATH_MODEL/all_metrik.csv
rm -rf ~/Dataset/NSL/$PATH_MODEL/dt_metrik.csv
rm -rf ~/Dataset/NSL/$PATH_MODEL/svm_metrik.csv
rm -rf ~/Dataset/NSL/$PATH_MODEL/dtsvm_metrik.csv

cp -rf model/$PATH_MODEL/model* ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/train ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/test ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/dtsvm_model.csv ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/hasil.txt ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/all_metrik.csv ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/dt_metrik.csv ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/svm_metrik.csv ~/Dataset/NSL/$PATH_MODEL
cp -rf model/$PATH_MODEL/dtsvm_metrik.csv ~/Dataset/NSL/$PATH_MODEL


