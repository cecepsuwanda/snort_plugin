#!/bin/bash


mkdir model
mkdir model/test
mkdir model/train
mkdir model/model1
mkdir model/model2
mkdir model/model3
mkdir model/model4

mkdir ~/Dataset/mit_darpa_98/model_100_2_svm_org

./my_dtsvm 0 1 0 0 100 2 0.0001 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model1 1 0 | tee ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait

# ./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model1  0 1 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
# wait

# ./my_dtsvm 1 0 0 0 100 2 0.0001 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model1  0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
# wait 

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model1 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model1 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 2 0.0003 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model2 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt 
wait

# ./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model2  0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
# wait

./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model2 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model2 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait



./my_dtsvm 2 1 0 0 100 2 0.0005 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model3 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt 
wait

# ./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model3  0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
# wait

./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model3 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model3 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait



./my_dtsvm 2 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model4 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt 
wait

# ./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model4  0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
# wait

./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model4 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model4 0 0 | tee -a ~/Dataset/mit_darpa_98/model_100_2_svm_org/hasil.txt
wait



rm -rf ~/Dataset/mit_darpa_98/model_100_2_svm_org/model*
rm -rf ~/Dataset/mit_darpa_98/model_100_2_svm_org/test
rm -rf ~/Dataset/mit_darpa_98/model_100_2_svm_org/train
rm -rf ~/Dataset/mit_darpa_98/model_100_2_svm_org/dtsvm_model.csv

cp -rf model/model* ~/Dataset/mit_darpa_98/model_100_2_svm_org
cp -rf model/train ~/Dataset/mit_darpa_98/model_100_2_svm_org
cp -rf model/test ~/Dataset/mit_darpa_98/model_100_2_svm_org
cp -rf model/dtsvm_model.csv ~/Dataset/mit_darpa_98/model_100_2_svm_org
