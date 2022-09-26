#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/19052021_0050/coba' 
NAMES='Dataset/NSL/kddcup_29attr.names'
TRAIN='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_train.txt'
TEST='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_test.txt'

DEPTH=100
MIN_SAMP=20
USE_CREDAL=1
CREDAL_S=2.0
LIMIT=1
THRESHOLD=100


mkdir ~/$PATH_MODEL

rm -rf ~/$PATH_MODEL/hasil.txt
rm -rf ~/$PATH_MODEL/dtsvm_model.csv
rm -rf ~/$PATH_MODEL/all_metrik.csv
rm -rf ~/$PATH_MODEL/dt_metrik.csv
rm -rf ~/$PATH_MODEL/svm_metrik.csv
rm -rf ~/$PATH_MODEL/dtsvm_metrik.csv

START="$(date +"%r")"
echo "START : $START"

./my_dt 0 $DEPTH $MIN_SAMP $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD ~/$NAMES ~/$TEST ~/$PATH_MODEL | tee ~/$PATH_MODEL/hasil.txt
wait

END="$(date +"%r")"
echo "START : $START END : $END"

# ./my_dt 1 $DEPTH $MIN_SAMP $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD ~/$NAMES ~/$TRAIN ~/$PATH_MODEL | tee -a ~/$PATH_MODEL/hasil.txt
# wait

# ./my_dt 1 $DEPTH $MIN_SAMP $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD ~/$NAMES ~/$TEST ~/$PATH_MODEL | tee -a ~/$PATH_MODEL/hasil.txt
# wait
