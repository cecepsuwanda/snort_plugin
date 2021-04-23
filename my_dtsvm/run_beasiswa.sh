#!/bin/bash

cd data
cp -rf ~/Dataset/beasiswa/*.* .
dos2unix *.*
cd ..

mkdir model/model_beasiswa
mkdir model/model_beasiswa/test
mkdir model/model_beasiswa/train
mkdir model/model_beasiswa/model1
mkdir model/model_beasiswa/model2
mkdir model/model_beasiswa/model3
mkdir model/model_beasiswa/model4

rm -rf model/model_beasiswa/hasil.txt
rm -rf model/model_beasiswa/dtsvm_model.csv
rm -rf model/model_beasiswa/all_metrik.csv
rm -rf model/model_beasiswa/dt_metrik.csv
rm -rf model/model_beasiswa/svm_metrik.csv
rm -rf model/model_beasiswa/dtsvm_metrik.csv


./my_dtsvm 0 0 0 0 10 1 0.0001 0.01  data/beasiswa.names data/beasiswa.csv model/model_beasiswa model1 0 0 0 0.0 | tee model/model_beasiswa/hasil.txt
wait

./my_dtsvm 1 0 0 0 10 1 0.0001 0.01 data/beasiswa.names data/beasiswa.csv model/model_beasiswa model1  0 0 0 0.0 | tee -a model/model_beasiswa/hasil.txt
wait



mkdir ~/Dataset/beasiswa/model_beasiswa

rm -rf ~/Dataset/beasiswa/model_beasiswa/model*
rm -rf ~/Dataset/beasiswa/model_beasiswa/test
rm -rf ~/Dataset/beasiswa/model_beasiswa/train
rm -rf ~/Dataset/beasiswa/model_beasiswa/dtsvm_model.csv
rm -rf ~/Dataset/beasiswa/model_beasiswa/hasil.txt
rm -rf ~/Dataset/beasiswa/model_beasiswa/all_metrik.csv
rm -rf ~/Dataset/beasiswa/model_beasiswa/dt_metrik.csv
rm -rf ~/Dataset/beasiswa/model_beasiswa/svm_metrik.csv
rm -rf ~/Dataset/beasiswa/model_beasiswa/dtsvm_metrik.csv

cp -rf model/model_beasiswa/model* ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/train ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/test ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/dtsvm_model.csv ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/hasil.txt ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/all_metrik.csv ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/dt_metrik.csv ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/svm_metrik.csv ~/Dataset/beasiswa/model_beasiswa
cp -rf model/model_beasiswa/dtsvm_metrik.csv ~/Dataset/beasiswa/model_beasiswa

