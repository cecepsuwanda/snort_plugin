#!/bin/bash

DEPTH_AWAL=6
DEPTH_AKHIR=30
DEPTH_STEP=1

MIN_SAMP_AWAL=2
MIN_SAMP_AKHIR=2
MIN_SAMP_STEP=1

CREDAL_S_AWAL=0.0
CREDAL_S_AKHIR=2.0
CREDAL_S_STEP=0.5

THRESHOLD_AWAL=100
THRESHOLD_AKHIR=100
THRESHOLD_STEP=100


ARR_ID_DT=(1 2 3 4 5)
ARR_JNS_DT=(4)

for ID_DT in "${ARR_ID_DT[@]}"; do
    
	START="$(date +"%r")"
	echo "START : $START"

	ID_DT_TRAIN=$ID_DT
	JNS_DT_TRAIN=1 
	PARTITION_TRAIN='p1'

	ID_DT_TEST=$ID_DT
	JNS_DT_TEST=1
	PARTITION_TEST='p1'

	./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST

	END="$(date +"%r")"
	echo "START : $START END : $END"
	wait

	for JNS_DT in "${ARR_JNS_DT[@]}"; do
         
		START="$(date +"%r")"
		echo "START : $START"

		ID_DT_TEST=$ID_DT
		JNS_DT_TEST=$JNS_DT
		PARTITION_TEST='p1'

		./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST

		END="$(date +"%r")"
		echo "START : $START END : $END"
		wait

	done

	START="$(date +"%r")"
	echo "START : $START"

	ID_DT_TEST=21
	JNS_DT_TEST=1
	PARTITION_TEST='p2'

	./my_dtdb $DEPTH_AWAL $DEPTH_AKHIR $DEPTH_STEP $MIN_SAMP_AWAL $MIN_SAMP_AKHIR $MIN_SAMP_STEP $CREDAL_S_AWAL $CREDAL_S_AKHIR $CREDAL_S_STEP $THRESHOLD_AWAL $THRESHOLD_AKHIR $THRESHOLD_STEP ~/$PATH_MODEL $ID_DT_TRAIN $JNS_DT_TRAIN $PARTITION_TRAIN $ID_DT_TEST $JNS_DT_TEST $PARTITION_TEST

	END="$(date +"%r")"
	echo "START : $START END : $END"
	wait

done
