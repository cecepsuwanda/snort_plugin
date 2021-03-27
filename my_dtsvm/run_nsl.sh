#!/bin/bash

mkdir model
mkdir model/test
mkdir model/train
mkdir model/model1
mkdir model/model2
mkdir model/model3
mkdir model/model4

mkdir ~/Dataset/NSL/coba

./my_dtsvm 0 1 0 0 100 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model1 0 0 1 | tee ~/Dataset/NSL/coba/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model1  0 0 1 | tee -a ~/Dataset/NSL/coba/hasil.txt
wait

./my_dtsvm 1 1 0 0 100 2 0.0001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model1 0 0 1 | tee -a ~/Dataset/NSL/coba/hasil.txt
wait 

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model1 0 0 1 | tee -a ~/Dataset/NSL/coba/hasil.txt 
wait

./my_dtsvm  1 1 0 0 100 2 0.0001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model1 0 0 1 | tee -a ~/Dataset/NSL/coba/hasil.txt
wait


# ./my_dtsvm 2 1 0 0 100 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt 
# wait

# ./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model2  0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait

# ./my_dtsvm 1 1 0 0 100 2 0.0003 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt 
# wait 

# ./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.0003 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model2 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait



# ./my_dtsvm 2 1 0 0 100 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt 
# wait

# ./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model3  0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait

# ./my_dtsvm 1 1 0 0 100 2 0.0005 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait 

# ./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.0005 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model3 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait



# ./my_dtsvm 2 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt 
# wait

# ./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_train.txt model model4  0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait

# ./my_dtsvm 1 1 0 0 100 2 0.001 0.01 data/kddcupSVM_33attr.names data/NSLSVM_33attr_test.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait 

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_test1.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait

# ./my_dtsvm  1 1 0 0 100 2 0.001 0.01  data/kddcupSVM_33attr.names data/NSLSVM_33attr_unknown.txt model model4 0 0 0 | tee -a ~/Dataset/NSL/model_100_2_svm_30/hasil.txt
# wait



rm -rf ~/Dataset/NSL/coba/model*
rm -rf ~/Dataset/NSL/coba/test
rm -rf ~/Dataset/NSL/coba/train
rm -rf ~/Dataset/NSL/coba/dtsvm_model.csv

cp -rf model/model* ~/Dataset/NSL/coba
cp -rf model/train ~/Dataset/NSL/coba
cp -rf model/test ~/Dataset/NSL/coba
cp -rf model/dtsvm_model.csv ~/Dataset/NSL/coba


