#!/bin/bash

PATH_MODEL='Dataset/mit_darpa_98/tree_dataset/28052021_1548/darpa98_model_40_700_svm_29attr_credal05_70_prunning' 
NAMES='Dataset/mit_darpa_98/kddcup_29attr.names'
TRAIN='Dataset/mit_darpa_98/tree_dataset/28052021_1548/darpa98_train.csv'
TEST='Dataset/mit_darpa_98/tree_dataset/28052021_1548/darpa98_test.csv'
TEST1='Dataset/mit_darpa_98/tree_dataset/28052021_1548/darpa98_test1.csv'
UNKNOWN='Dataset/mit_darpa_98/tree_dataset/28052021_1548/darpa98_unknown.csv'
VALIDITY='Dataset/mit_darpa_98/tree_dataset/28052021_1548/cek_validity.csv'

DEPTH=40
MIN_SAMP=700
USE_CREDAL=1
CREDAL_S=0.5
LIMIT=1
THRESHOLD=70
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

./my_dtsvm 0 0 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model1 1 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee ~/$PATH_MODEL/hasil.txt
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


# mkdir ~/Dataset/mit_darpa_98/$PATH_MODEL

# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/model*
# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/train
# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/dtsvm_model.csv

# cp -rf model/$PATH_MODEL/model* ~/Dataset/mit_darpa_98/$PATH_MODEL
# cp -rf model/$PATH_MODEL/train ~/Dataset/mit_darpa_98/$PATH_MODEL
# cp -rf model/$PATH_MODEL/dtsvm_model.csv ~/Dataset/mit_darpa_98/$PATH_MODEL

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model1 0 1 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait

# rm -rf ~/Dataset/mit_darpa_98/$PATH_~/test
# cp -rf model/$PATH_MODEL/test ~/Dataset/mit_darpa_98/$PATH_MODEL


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model1  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  ~/$NAMES ~/$VALIDITY ~/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model2  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt 
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  ~/$NAMES ~/$VALIDITY ~/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model3  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  ~/$NAMES ~/$VALIDITY ~/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model4  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  ~/$NAMES ~/$VALIDITY ~/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model5  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  ~/$NAMES ~/$VALIDITY ~/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01 ~/$NAMES ~/$TRAIN ~/$PATH_MODEL model6  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01 ~/$NAMES ~/$TEST ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  ~/$NAMES ~/$TEST1 ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  ~/$NAMES ~/$UNKNOWN ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  ~/$NAMES ~/$VALIDITY ~/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a ~/$PATH_MODEL/hasil.txt
wait

END="$(date +"%r")"
echo "START : $START END : $END"

# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/hasil.txt
# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/all_metrik.csv
# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/dt_metrik.csv
# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/svm_metrik.csv
# rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/dtsvm_metrik.csv

# cp -rf model/$PATH_MODEL/hasil.txt ~/Dataset/mit_darpa_98/$PATH_MODEL
# cp -rf model/$PATH_MODEL/all_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL
# cp -rf model/$PATH_MODEL/dt_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL
# cp -rf model/$PATH_MODEL/svm_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL
# cp -rf model/$PATH_MODEL/dtsvm_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL