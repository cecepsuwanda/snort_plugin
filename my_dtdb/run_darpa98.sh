#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/tmp/coba' 

ID_DT_TRAIN=21
JNS_DT_TRAIN=1 
PARTITION_TRAIN='p2'

ID_DT_TEST=21
JNS_DT_TEST=1
PARTITION_TEST='p2'


DEPTH_AWAL=11
DEPTH_AKHIR=11
DEPTH_STEP=1

MIN_SAMP_AWAL=100
MIN_SAMP_AKHIR=100
MIN_SAMP_STEP=2

CREDAL_S_AWAL=0.0
CREDAL_S_AKHIR=2.0
CREDAL_S_STEP=0.5

THRESHOLD_AWAL=0.1
THRESHOLD_AKHIR=0.1
THRESHOLD_STEP=0.1

# mkdir ~/$PATH_MODEL

START="$(date +"%r")"
echo "START : $START"

# ./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST
# wait

ID_DT_TEST=21
JNS_DT_TEST=2
PARTITION_TEST='p2'

# ./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST
# wait

ID_DT_TEST=211
JNS_DT_TEST=1
PARTITION_TEST='p2'

# ./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST
# wait

ID_DT_TEST=211
JNS_DT_TEST=2
PARTITION_TEST='p2'

./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST
wait

END="$(date +"%r")"
echo "START : $START END : $END"


