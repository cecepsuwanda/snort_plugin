#!/bin/bash

PATH_MODEL='Dataset/mit_darpa_98/tree_dataset/28052021_1548/coba' 
NAMES='Dataset/mit_darpa_98/kddcup_29attr.names'
TRAIN='Dataset/mit_darpa_98/tree_dataset/28052021_1548/darpa98_test.csv'
TEST='Dataset/mit_darpa_98/tree_dataset/28052021_1548/darpa98_train.csv'

DEPTH_AWAL=32
DEPTH_AKHIR=32
DEPTH_STEP=1

MIN_SAMP_AWAL=2
MIN_SAMP_AKHIR=2
MIN_SAMP_STEP=1

CREDAL_S_AWAL=0.0
CREDAL_S_AKHIR=0.0
CREDAL_S_STEP=0.5

THRESHOLD_AWAL=2
THRESHOLD_AKHIR=2
THRESHOLD_STEP=1

mkdir ~/$PATH_MODEL
rm ~/$PATH_MODEL/dtsvm_model_*.csv

START="$(date +"%r")"
echo "START : $START"

./my_dt $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$NAMES ~/$TRAIN ~/$TRAIN ~/$PATH_MODEL | tee ~/$PATH_MODEL/experiment1.txt
wait

END="$(date +"%r")"
echo "START : $START END : $END"

rm ~/$PATH_MODEL/dtsvm_model_*.csv
