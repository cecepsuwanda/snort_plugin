#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/tmp/coba' 
NAMES='Dataset/NSL/kddcup_29attr.names'
TRAIN='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_train.txt'
TEST='Dataset/NSL/tree_dataset/tmp/NSLTree_29attr_test.txt'

DEPTH_AWAL=16
DEPTH_AKHIR=50
DEPTH_STEP=1

MIN_SAMP_AWAL=2
MIN_SAMP_AKHIR=2
MIN_SAMP_STEP=1

CREDAL_S_AWAL=0.0
CREDAL_S_AKHIR=0.0
CREDAL_S_STEP=0.1

THRESHOLD_AWAL=2
THRESHOLD_AKHIR=2
THRESHOLD_STEP=1

# mkdir ~/$PATH_MODEL

START="$(date +"%r")"
echo "START : $START"

./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$NAMES ~/$TRAIN ~/$TRAIN ~/$PATH_MODEL
wait

END="$(date +"%r")"
echo "START : $START END : $END"


