#!/bin/bash

PATH_MODEL='darpa98_model_40_700_svm_29attr_credal1' 
NAMES='data/kddcup_29attr.names'
TRAIN='Dataset/mit_darpa_98/train/label/darpa98_train.csv'
TEST='Dataset/mit_darpa_98/train/label/darpa98_test.csv'
TEST1='Dataset/mit_darpa_98/train/label/darpa98_test1.csv'
UNKNOWN='Dataset/mit_darpa_98/train/label/darpa98_unknown.csv'
DEPTH=40
MIN_SAMP=700
USE_CREDAL=1
CREDAL_S=1.0
LIMIT=1
THRESHOLD=70
F=0
N=0

mkdir model/$PATH_MODEL
mkdir model/$PATH_MODEL/test
mkdir model/$PATH_MODEL/train
mkdir model/$PATH_MODEL/model1
mkdir model/$PATH_MODEL/model2
mkdir model/$PATH_MODEL/model3
mkdir model/$PATH_MODEL/model4
mkdir model/$PATH_MODEL/model5
mkdir model/$PATH_MODEL/model6


rm -rf model/$PATH_MODEL/hasil.txt
rm -rf model/$PATH_MODEL/dtsvm_model.csv
rm -rf model/$PATH_MODEL/all_metrik.csv
rm -rf model/$PATH_MODEL/dt_metrik.csv
rm -rf model/$PATH_MODEL/svm_metrik.csv
rm -rf model/$PATH_MODEL/dtsvm_metrik.csv

START="$(date +"%r")"
echo "START : $START"

./my_dtsvm 0 0 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  $NAMES ~/$TRAIN model/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  $NAMES ~/$TRAIN model/$PATH_MODEL model1 1 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  $NAMES ~/$TRAIN model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  $NAMES ~/$TRAIN model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  $NAMES ~/$TRAIN model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  $NAMES ~/$TRAIN model/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt 
wait

./my_dtsvm 2 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  $NAMES ~/$TRAIN model/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt 
wait


mkdir ~/Dataset/mit_darpa_98/$PATH_MODEL

rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/model*
rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/train
rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/dtsvm_model.csv

cp -rf model/$PATH_MODEL/model* ~/Dataset/mit_darpa_98/$PATH_MODEL
cp -rf model/$PATH_MODEL/train ~/Dataset/mit_darpa_98/$PATH_MODEL
cp -rf model/$PATH_MODEL/dtsvm_model.csv ~/Dataset/mit_darpa_98/$PATH_MODEL

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  $NAMES ~/$TEST1 model/$PATH_MODEL model1 0 1 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt 
wait

rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/test
cp -rf model/$PATH_MODEL/test ~/Dataset/mit_darpa_98/$PATH_MODEL


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 $NAMES ~/$TRAIN model/$PATH_MODEL model1  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01 $NAMES ~/$TEST model/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0001 0.01  $NAMES ~/$UNKNOWN model/$PATH_MODEL model1 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 $NAMES ~/$TRAIN model/$PATH_MODEL model2  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01 $NAMES ~/$TEST model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt 
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  $NAMES ~/$TEST1 model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0003 0.01  $NAMES ~/$UNKNOWN model/$PATH_MODEL model2 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait


./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 $NAMES ~/$TRAIN model/$PATH_MODEL model3  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01 $NAMES ~/$TEST model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  $NAMES ~/$TEST1 model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.0005 0.01  $NAMES ~/$UNKNOWN model/$PATH_MODEL model3 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 $NAMES ~/$TRAIN model/$PATH_MODEL model4  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01 $NAMES ~/$TEST model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  $NAMES ~/$TEST1 model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.001 0.01  $NAMES ~/$UNKNOWN model/$PATH_MODEL model4 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01 $NAMES ~/$TRAIN model/$PATH_MODEL model5  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01 $NAMES ~/$TEST model/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  $NAMES ~/$TEST1 model/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.003 0.01  $NAMES ~/$UNKNOWN model/$PATH_MODEL model5 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait



./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01 $NAMES ~/$TRAIN model/$PATH_MODEL model6  0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm 1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01 $NAMES ~/$TEST model/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait 

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  $NAMES ~/$TEST1 model/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

./my_dtsvm  1 1 $F $N $DEPTH $MIN_SAMP 0.004 0.01  $NAMES ~/$UNKNOWN model/$PATH_MODEL model6 0 0 $USE_CREDAL $CREDAL_S $LIMIT $THRESHOLD | tee -a model/$PATH_MODEL/hasil.txt
wait

END="$(date +"%r")"
echo "START : $START END : $END"

rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/hasil.txt
rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/all_metrik.csv
rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/dt_metrik.csv
rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/svm_metrik.csv
rm -rf ~/Dataset/mit_darpa_98/$PATH_MODEL/dtsvm_metrik.csv

cp -rf model/$PATH_MODEL/hasil.txt ~/Dataset/mit_darpa_98/$PATH_MODEL
cp -rf model/$PATH_MODEL/all_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL
cp -rf model/$PATH_MODEL/dt_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL
cp -rf model/$PATH_MODEL/svm_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL
cp -rf model/$PATH_MODEL/dtsvm_metrik.csv ~/Dataset/mit_darpa_98/$PATH_MODEL