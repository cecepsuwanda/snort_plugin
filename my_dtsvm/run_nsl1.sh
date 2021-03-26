#!/bin/bash

mkdir model
mkdir model/test
mkdir model/train
mkdir model/model1
mkdir model/model2
mkdir model/model3
mkdir model/model4

mkdir ~/Dataset/NSL/model_100_2_svm_org_30_f

./my_dtsvm 0 1 1 0 100 2 0.0001 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model model1 0 0 | tee ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait

./my_dtsvm 1 1 1 0 100 2 0.0001 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model model1  0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait

./my_dtsvm 1 0 0 0 100 2 0.0001 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model model1  0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait 

./my_dtsvm 1 1 1 0 100 2 0.0001 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model model1 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait 

./my_dtsvm  1 1 1 0 100 2 0.0001 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model model1 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt 
wait

./my_dtsvm  1 1 1 0 100 2 0.0001 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model model1 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait


./my_dtsvm 2 1 1 0 100 2 0.0003 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model model2 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt 
wait

# ./my_dtsvm 1 1 1 0 100 2 0.0003 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model model2  0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
# wait

./my_dtsvm 1 1 1 0 100 2 0.0003 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model model2 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt 
wait 

./my_dtsvm  1 1 1 0 100 2 0.0003 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model model2 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait

./my_dtsvm  1 1 1 0 100 2 0.0003 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model model2 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait



./my_dtsvm 2 1 1 0 100 2 0.0005 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model model3 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt 
wait

# ./my_dtsvm 1 1 1 0 100 2 0.0005 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model model3  0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
# wait

./my_dtsvm 1 1 1 0 100 2 0.0005 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model model3 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait 

./my_dtsvm  1 1 1 0 100 2 0.0005 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model model3 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait

./my_dtsvm  1 1 1 0 100 2 0.0005 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model model3 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait



./my_dtsvm 2 1 1 0 100 2 0.001 0.01  data/kddcupSVM.names data/NSLSVM_train.txt model model4 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt 
wait

# ./my_dtsvm 1 1 1 0 100 2 0.001 0.01 data/kddcupSVM.names data/NSLSVM_train.txt model model4  0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
# wait

./my_dtsvm 1 1 1 0 100 2 0.001 0.01 data/kddcupSVM.names data/NSLSVM_test.txt model model4 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait 

./my_dtsvm  1 1 1 0 100 2 0.001 0.01  data/kddcupSVM.names data/NSLSVM_test1.txt model model4 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait

./my_dtsvm  1 1 1 0 100 2 0.001 0.01  data/kddcupSVM.names data/NSLSVM_unknown.txt model model4 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_org_30_f/hasil.txt
wait



rm -rf ~/Dataset/NSL/model_100_2_svm_org_30_f/model*
rm -rf ~/Dataset/NSL/model_100_2_svm_org_30_f/test
rm -rf ~/Dataset/NSL/model_100_2_svm_org_30_f/train
rm -rf ~/Dataset/NSL/model_100_2_svm_org_30_f/dtsvm_model.csv

cp -rf model/model* ~/Dataset/NSL/model_100_2_svm_org_30_f
cp -rf model/train ~/Dataset/NSL/model_100_2_svm_org_30_f
cp -rf model/test ~/Dataset/NSL/model_100_2_svm_org_30_f
cp -rf model/dtsvm_model.csv ~/Dataset/NSL/model_100_2_svm_org_30_f


