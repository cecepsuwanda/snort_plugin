#!/bin/bash

mkdir model
mkdir model/model1
rm model/model1/dtsvm_model*.csv
rm model/model1/svm_model*.csv
wait

./my_dtsvm  15 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model1 1 1 0 0
wait

./my_dtsvm  15 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model1 0 1 0 0
wait

./my_dtsvm  15 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model1 0 1 0 0
wait

# mkdir model/model2
# rm model/model2/dtsvm_model*.csv
# rm model/model2/svm_model*.csv
# wait

# ./my_dtsvm  15 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model2 1 0 0 0
# wait

# ./my_dtsvm  15 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model2 0 0 0 0
# wait

# ./my_dtsvm  15 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model2 0 0 0 0
# wait

# mkdir model/model3
# rm model/model3/dtsvm_model*.csv
# rm model/model3/svm_model*.csv
# wait

# ./my_dtsvm  15 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model3 1 0 0 0
# wait

# ./my_dtsvm  15 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model3 0 0 0 0
# wait

# ./my_dtsvm  15 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model3 0 0 0 0
# wait

# mkdir model/model4
# rm model/model4/dtsvm_model*.csv
# rm model/model4/svm_model*.csv
# wait

# ./my_dtsvm  15 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test.txt model/model4 1 0 0 0
# wait

# ./my_dtsvm  15 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_test1.txt model/model4 0 0 0 0
# wait

# ./my_dtsvm  15 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt data/NSLSVM_33attr_unknown.txt model/model4 0 0 0 0
# wait


