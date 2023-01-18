#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/tmp/coba' 

ID_DT_TRAIN=2
JNS_DT_TRAIN=1 
PARTITION_TRAIN='p21'

ID_DT_TEST=2
JNS_DT_TEST=1
PARTITION_TEST='p21'


DEPTH_AWAL=48
DEPTH_AKHIR=48
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

# mkdir ~/$PATH_MODEL

START="$(date +"%r")"
echo "START : $START"

./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST
wait

END="$(date +"%r")"
echo "START : $START END : $END"


