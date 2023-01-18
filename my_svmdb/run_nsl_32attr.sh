#!/bin/bash

PATH_MODEL='Dataset/NSL/tree_dataset/19052021_0050'
NAMES='Dataset/NSL/kddcupSVM_32attr.names'
MODEL00='model_20_2_svm_29attr_2_prunning'
MODEL05='model_20_2_svm_29attr_credal05_2_prunning'
MODEL10='model_20_2_svm_29attr_credal1_2_prunning' 

GAMMA_AWAL=0.0001
GAMMA_AKHIR=0.0005
GAMMA_STEP=0.0001

NU_AWAL=0.01
NU_AKHIR=1.0
NU_STEP=0.01

# mkdir ~/$PATH_MODEL/$MODEL00/model_opt

./my_svmdb ~/$PATH_MODEL/$MODEL00 ~/$NAMES $GAMMA_AWAL $GAMMA_AKHIR $GAMMA_STEP $NU_AWAL $NU_AKHIR $NU_STEP model_opt  #| tee ~/$PATH_MODEL/$MODEL00/cari_gamma_nu.txt
wait

# mkdir ~/$PATH_MODEL/$MODEL05/model_opt

# ./my_svmdb ~/$PATH_MODEL/$MODEL05 ~/$NAMES $GAMMA_AWAL $GAMMA_AKHIR $GAMMA_STEP $NU_AWAL $NU_AKHIR $NU_STEP model_opt | tee ~/$PATH_MODEL/$MODEL05/cari_gamma_nu.txt
# wait

# mkdir ~/$PATH_MODEL/$MODEL10/model_opt

# ./my_svmdb ~/$PATH_MODEL/$MODEL10 ~/$NAMES $GAMMA_AWAL $GAMMA_AKHIR $GAMMA_STEP $NU_AWAL $NU_AKHIR $NU_STEP model_opt | tee ~/$PATH_MODEL/$MODEL10/cari_gamma_nu.txt
# wait



