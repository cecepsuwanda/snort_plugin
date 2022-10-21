#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/tmp/coba' 
NAMES='Dataset/NSL/kddcup_29attr.names'
TRAIN='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_train.txt'
TEST='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_test.txt'

DEPTH_AWAL=10
DEPTH_AKHIR=50
DEPTH_STEP=1

MIN_SAMP_AWAL=10
MIN_SAMP_AKHIR=100
MIN_SAMP_STEP=5

CREDAL_S_AWAL=0.0
CREDAL_S_AKHIR=1.0
CREDAL_S_STEP=0.5

THRESHOLD_AWAL=10
THRESHOLD_AKHIR=100
THRESHOLD_STEP=5


mkdir ~/$PATH_MODEL
rm ~/$PATH_MODEL/dtsvm_model_*.csv

START="$(date +"%r")"
echo "START : $START"

./my_dt $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$NAMES ~/$TRAIN ~/$TRAIN ~/$PATH_MODEL | tee ~/$PATH_MODEL/coba_prunning_1.txt
wait

END="$(date +"%r")"
echo "START : $START END : $END"

rm ~/$PATH_MODEL/dtsvm_model_*.csv
