#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/19052021_0050/model_6_200_svm_29attr_credal1_100_prunning' 
NAMES='Dataset/NSL/kddcup_29attr.names'
TRAIN='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_train.txt'
TEST='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_test.txt'
TEST1='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_test1.txt'
UNKNOWN='Dataset/NSL/tree_dataset/19052021_0050/NSLTree_29attr_unknown.txt'

DEPTH=6
MIN_SAMP=200
USE_CREDAL=1
CREDAL_S=1.0
LIMIT=1
THRESHOLD=100
F=0
N=0

mkdir ~/$PATH_MODEL
mkdir ~/$PATH_MODEL/test
mkdir ~/$PATH_MODEL/train
mkdir ~/$PATH_MODEL/model1
mkdir ~/$PATH_MODEL/model2
mkdir ~/$PATH_MODEL/model3
mkdir ~/$PATH_MODEL/model4
mkdir ~/$PATH_MODEL/model5
mkdir ~/$PATH_MODEL/model6


rm -rf ~/$PATH_MODEL/hasil.txt
rm -rf ~/$PATH_MODEL/dtsvm_model.csv
rm -rf ~/$PATH_MODEL/all_metrik.csv
rm -rf ~/$PATH_MODEL/dt_metrik.csv
rm -rf ~/$PATH_MODEL/svm_metrik.csv
rm -rf ~/$PATH_MODEL/dtsvm_metrik.csv

START="$(date +"%r")"
echo "START : $START"

./my_dtsvm 0 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model1 1 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait


# mkdir ~/Dataset/NSL/$PATH_MODEL

# rm -rf ~/Dataset/NSL/$PATH_MODEL/model*
# rm -rf ~/Dataset/NSL/$PATH_MODEL/train
# rm -rf ~/Dataset/NSL/$PATH_MODEL/dtsvm_model.csv

# cp -rf model/$PATH_MODEL/model* ~/Dataset/NSL/$PATH_MODEL
# cp -rf model/$PATH_MODEL/train ~/Dataset/NSL/$PATH_MODEL
# cp -rf model/$PATH_MODEL/dtsvm_model.csv ~/Dataset/NSL/$PATH_MODEL

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model1 0 1 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait

# rm -rf ~/Dataset/NSL/$PATH_MODEL/test
# cp -rf model/$PATH_MODEL/test ~/Dataset/NSL/$PATH_MODEL


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model1  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model2  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model3  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model4  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model5  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model6  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

END="$(date +"%r")"
echo "START : $START END : $END"

# rm -rf ~/Dataset/NSL/$PATH_MODEL/hasil.txt
# rm -rf ~/Dataset/NSL/$PATH_MODEL/all_metrik.csv
# rm -rf ~/Dataset/NSL/$PATH_MODEL/dt_metrik.csv
# rm -rf ~/Dataset/NSL/$PATH_MODEL/svm_metrik.csv
# rm -rf ~/Dataset/NSL/$PATH_MODEL/dtsvm_metrik.csv

# cp -rf model/$PATH_MODEL/hasil.txt ~/Dataset/NSL/$PATH_MODEL
# cp -rf model/$PATH_MODEL/all_metrik.csv ~/Dataset/NSL/$PATH_MODEL
# cp -rf model/$PATH_MODEL/dt_metrik.csv ~/Dataset/NSL/$PATH_MODEL
# cp -rf model/$PATH_MODEL/svm_metrik.csv ~/Dataset/NSL/$PATH_MODEL
# cp -rf model/$PATH_MODEL/dtsvm_metrik.csv ~/Dataset/NSL/$PATH_MODEL