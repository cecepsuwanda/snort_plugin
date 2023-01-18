#!/bin/bash

cd ~/cpp/my_svmdb
cp -rf ~/snort_plugin/my_svmdb/makefile .
cp -rf ~/snort_plugin/my_svmdb/src/*.* ./src
make
# wait
# clear
# wait

#rm data/svm_model.csv
#wait

#./my_svm ~/Dataset/NSL/model_100_600_svm_42attr_normal ~/cpp/my_dtsvm/data/kddcupSVM.names

# ./my_svm  0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model1.csv
# wait

# ./my_svm  0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model2.csv
# wait

# ./my_svm  0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model3.csv
# wait

# ./my_svm  0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train_oneclass.txt data/NSLSVM_33attr_test_oneclass.txt data/svm_model4.csv
# wait
