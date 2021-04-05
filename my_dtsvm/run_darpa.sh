#!/bin/bash


mkdir model
mkdir model/test
mkdir model/train
mkdir model/model1
mkdir model/model2
mkdir model/model3
mkdir model/model4



./my_dtsvm 0 1 0 0 100 4200 0.0001 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model1 0 0 0 0.5 | tee model/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 4200 0.0001 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model1  0 0 0 0.5 | tee -a model/hasil.txt
wait


./my_dtsvm 1 1 0 0 100 4200 0.0001 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model1 0 0 0 0.5 | tee -a model/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 4200 0.0001 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model1 0 0 0 0.5 | tee -a model/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 4200 0.0003 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model2 0 0 0 0.5 | tee -a model/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 4200 0.0003 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model2  0 0 0 0.5 | tee -a model/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 4200 0.0003 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model2 0 0 0 0.5 | tee -a model/hasil.txt 
wait 

./my_dtsvm  1 1 0 0 100 4200 0.0003 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model2 0 0 0 0.5 | tee -a model/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 4200 0.0005 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model3 0 0 0 0.5 | tee -a model/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 4200 0.0005 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model3  0 0 0 0.5 | tee -a model/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 4200 0.0005 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model3 0 0 0 0.5 | tee -a model/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 4200 0.0005 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model3 0 0 0 0.5 | tee -a model/hasil.txt
wait


./my_dtsvm 2 1 0 0 100 4200 0.001 0.01  data/kddcupSVM_33attr.names data/darpa98_train.csv model model4 0 0 0 0.5 | tee -a model/hasil.txt 
wait

./my_dtsvm 1 1 0 0 100 4200 0.001 0.01 data/kddcupSVM_33attr.names data/darpa98_train.csv model model4  0 0 0 0.5 | tee -a model/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 4200 0.001 0.01 data/kddcupSVM_33attr.names data/darpa98_test.csv model model4 0 0 0 0.5 | tee -a model/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 4200 0.001 0.01  data/kddcupSVM_33attr.names data/darpa98_unknown.csv model model4 0 0 0 0.5 | tee -a model/hasil.txt
wait

mkdir ~/Dataset/mit_darpa_98/model_100_4200_svm

rm -rf ~/Dataset/mit_darpa_98/model_100_4200_svm/model*
rm -rf ~/Dataset/mit_darpa_98/model_100_4200_svm/test
rm -rf ~/Dataset/mit_darpa_98/model_100_4200_svm/train
rm -rf ~/Dataset/mit_darpa_98/model_100_4200_svm_/dtsvm_model.csv
rm -rf ~/Dataset/mit_darpa_98/model_100_4200_svm/hasil.txt

cp -rf model/model* ~/Dataset/mit_darpa_98/model_100_4200_svm
cp -rf model/train ~/Dataset/mit_darpa_98/model_100_4200_svm
cp -rf model/test ~/Dataset/mit_darpa_98/model_100_4200_svm
cp -rf model/dtsvm_model.csv ~/Dataset/mit_darpa_98/model_100_4200_svm
cp -rf model/hasil.txt ~/Dataset/mit_darpa_98/model_100_4200_svm

# rm -rf model/model*
# rm -rf model/hasil.txt
# rm -rf model/dtsvm_model.csv
# rm -rf model/train
# rm -rf model/test
# rm -rf model/hasil_model*.txt

# cp -rf ~/Dataset/mit_darpa_98/model_100_2_svm_credal1/model* model
# cp -rf ~/Dataset/mit_darpa_98/model_100_2_svm_credal1/dtsvm_model.csv model


# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_4w_thursday_test.csv model model4 0 0 1 1.0 | tee  model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_4w_tuesday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_4w_wednesday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_4w_friday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait


# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_5w_monday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_5w_thursday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait
 
# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_5w_tuesday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_5w_wednesday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_5w_friday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait


# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_6w_monday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_6w_thursday_test.csv  model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_6w_tuesday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_6w_wednesday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_6w_friday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait


# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_7w_monday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_7w_thursday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_7w_tuesday_test.csv model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_7w_wednesday_test.csv  model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names ~/Dataset/mit_darpa_98/label2/darpa98_7w_friday_test.csv  model model4 0 0 1 1.0 | tee -a model/hasil_model4.txt
# wait

# cp -rf model/hasil_model4.txt ~/Dataset/mit_darpa_98/model_100_2_svm_credal1