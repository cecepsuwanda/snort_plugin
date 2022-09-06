#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/tmp/model_100_100_tree_29attr_100' 
NAMES='Dataset/NSL/kddcup_29attr.names'
TRAIN='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_train.txt'
TEST='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_test.txt'

DEPTH=100
MIN_SAMP=100
USE_CREDAL=0
CREDAL_S=0.0
LIMIT=1
THRESHOLD=100


mkdir ~/$PATH_MODEL

rm -rf ~/$PATH_MODEL/hasil.txt
rm -rf ~/$PATH_MODEL/dtsvm_model.csv
rm -rf ~/$PATH_MODEL/all_metrik.csv
rm -rf ~/$PATH_MODEL/dt_metrik.csv
rm -rf ~/$PATH_MODEL/svm_metrik.csv
rm -rf ~/$PATH_MODEL/dtsvm_metrik.csv

./my_dt 0 $DEPTH $MIN_SAMP $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD ~/$NAMES ~/$TRAIN ~/$PATH_MODEL | tee ~/$PATH_MODEL/hasil.txt
wait

./my_dt 1 $DEPTH $MIN_SAMP $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD ~/$NAMES ~/$TRAIN ~/$PATH_MODEL | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dt 1 $DEPTH $MIN_SAMP $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD ~/$NAMES ~/$TEST ~/$PATH_MODEL | tee -a ~/$PATH_MODEL/hasil.txt
wait
