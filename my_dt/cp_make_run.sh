#!/bin/bash

cd ~/cpp/my_dt
cp -rf /mnt/e/snort_plugin/my_dt/makefile .
cp -rf /mnt/e/snort_plugin/my_dt/Tread_file.* .
cp -rf /mnt/e/snort_plugin/my_dt/Tdataframe.* .
cp -rf /mnt/e/snort_plugin/my_dt/Tdec_tree.* .
cp -rf /mnt/e/snort_plugin/my_dt/Tconf_metrix.* .
cp -rf /mnt/e/snort_plugin/my_dt/main.cc .
make
# wait
clear
# wait

rm data/dc_tree_dp*.csv
wait

# ./my_dt 36 data/kddcup.names data/NSLTrainTree.txt data/NSLTestTree.txt data/dc_tree_dp36.csv
# wait

# ./my_dt 36 data/kddcup.names data/NSLTrainTree_known.txt data/NSLTestTree_known.txt data/dc_tree_dp36.csv
# wait

#./my_dt 36 data/kddcup.names data/NSLTrainTree_known.txt data/NSLTestTree_unknown.txt data/dc_tree_dp36.csv
# wait

./my_dt 36 data/kddcupSVM.names data/NSLSVM_train.txt data/NSLSVM_test.txt data/dc_tree_dp36.csv
wait

./my_dt 36 data/kddcupSVM.names data/NSLSVM_train.txt data/NSLSVM_unknown.txt data/dc_tree_dp36.csv
wait
