#!/bin/bash

mkdir model
mkdir model/model1
rm model/model1/dtsvm_model*.csv
rm model/model1/svm_model*.csv
wait

./my_dtsvm  30 2 0.0001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label/darpa98_train.csv ~/Dataset/mit_darpa_98/label/darpa98_test.csv model/model1 1
wait

mkdir model/model2
rm model/model2/dtsvm_model*.csv
rm model/model2/svm_model*.csv
wait

./my_dtsvm  30 2 0.0003 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label/darpa98_train.csv ~/Dataset/mit_darpa_98/label/darpa98_test.csv model/model2 1
wait

mkdir model/model3
rm model/model3/dtsvm_model*.csv
rm model/model3/svm_model*.csv
wait

./my_dtsvm  30 2 0.0005 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label/darpa98_train.csv ~/Dataset/mit_darpa_98/label/darpa98_test.csv model/model3 1
wait

mkdir model/model4
rm model/model4/dtsvm_model*.csv
rm model/model4/svm_model*.csv
wait

./my_dtsvm  30 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label/darpa98_train.csv ~/Dataset/mit_darpa_98/label/darpa98_test.csv model/model4 1
wait


