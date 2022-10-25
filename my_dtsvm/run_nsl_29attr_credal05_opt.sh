#!/bin/bash

# PATH_MODEL='Dataset/NSL/tree_dataset/tmp/model_50_2_svm_29attr_credal05_10_prunning' 
# NAMES='Dataset/NSL/kddcup_29attr.names'
# TRAIN='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_train.txt'
# TEST='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_test.txt'
# TEST1='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_test1.txt'
# UNKNOWN='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_unknown.txt'

PATH_MODEL='Dataset/NSL/tree_dataset/19052021_0050/model_9_100_svm_29attr_credal05_1_prunning_opt' 
NAMES='Dataset/NSL/kddcup_29attr.names'
TRAIN='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_train.txt'
TEST='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_test.txt'
TEST1='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_test1.txt'
UNKNOWN='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_unknown.txt'

DEPTH=9
MIN_SAMP=100
USE_CREDAL=1
CREDAL_S=0.5
LIMIT=1
THRESHOLD=1
F=0
N=0

mkdir ~/$PATH_MODEL
mkdir ~/$PATH_MODEL/test
mkdir ~/$PATH_MODEL/train

rm -rf ~/$PATH_MODEL/hasil.txt
rm -rf ~/$PATH_MODEL/all_metrik.csv
rm -rf ~/$PATH_MODEL/dt_metrik.csv
rm -rf ~/$PATH_MODEL/svm_metrik.csv
rm -rf ~/$PATH_MODEL/dtsvm_metrik.csv

START="$(date +"%r")"
echo "START : $START"

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model_opt  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model_opt 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model_opt 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model_opt 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

END="$(date +"%r")"
echo "START : $START END : $END"
