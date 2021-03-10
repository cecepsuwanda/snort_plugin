#!/bin/bash

mkdir model
mkdir model/test
mkdir model/train
mkdir model/model1
mkdir model/model2
mkdir model/model3
mkdir model/model4

./my_dtsvm 0 1 0 0 26 1000 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model1 
wait

./my_dtsvm 1 1 0 0 26 1000 0.0001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model1
wait 

# ./my_dtsvm 1 1 0 0 26 2 0.0001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model1
# wait 

# ./my_dtsvm  1 1 0 0 26 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model1
# wait

# ./my_dtsvm  1 1 0 0 26 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model1
# wait


# ./my_dtsvm 2 1 0 0 26 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model2 
# wait

# ./my_dtsvm 1 1 0 0 26 2 0.0003 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model2
# wait 

# ./my_dtsvm  1 1 0 0 26 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model2
# wait

# ./my_dtsvm  1 1 0 0 26 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model2
# wait



# ./my_dtsvm 2 1 0 0 26 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model3 
# wait

# ./my_dtsvm 1 1 0 0 26 2 0.0005 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model3
# wait 

# ./my_dtsvm  1 1 0 0 26 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model3
# wait

# ./my_dtsvm  1 1 0 0 26 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model3
# wait



# ./my_dtsvm 2 1 0 0 26 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model4 
# wait

# ./my_dtsvm 1 1 0 0 26 2 0.001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model4
# wait 

# ./my_dtsvm  1 1 0 0 26 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model4
# wait

# ./my_dtsvm  1 1 0 0 26 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model4
# wait


# mkdir ~/Dataset/NSL/model_26_2_svm
# rm -rf ~/Dataset/NSL/model_26_2_svm/model*
# rm -rf ~/Dataset/NSL/model_26_2_svm/test
# rm -rf ~/Dataset/NSL/model_26_2_svm/train
# rm -rf ~/Dataset/NSL/model_26_2_svm/dtsvm_model.csv

# cp -rf model/model* ~/Dataset/NSL/model_26_2_svm
# cp -rf model/train ~/Dataset/NSL/model_26_2_svm
# cp -rf model/test ~/Dataset/NSL/model_26_2_svm
# cp -rf model/dtsvm_model.csv ~/Dataset/NSL/model_26_2_svm


