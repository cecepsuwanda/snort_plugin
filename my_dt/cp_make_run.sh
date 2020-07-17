#!/bin/bash

cd ~/cpp/my_dt
cp -rf /mnt/e/snort_plugin/my_dt/Tread_file.* .
cp -rf /mnt/e/snort_plugin/my_dt/Tdataframe.* .
cp -rf /mnt/e/snort_plugin/my_dt/Tdec_tree.* .
cp -rf /mnt/e/snort_plugin/my_dt/main.cc .
make
# wait
clear
# wait

# train_path = `data/train_corrected.csv`;
# test_path =  `data/test_corrected.csv`;
# name_path = `data/train.names`;

# rm data/dc_full_tree_*.csv
# wait

./my_dt 5 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp5.csv
wait
./my_dt 6 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp6.csv
wait
./my_dt 7 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp7.csv
wait
./my_dt 8 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp8.csv
wait
./my_dt 9 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp9.csv
wait
./my_dt 10 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp10.csv
wait
./my_dt 11 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp11.csv
wait
./my_dt 12 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp12.csv
wait
./my_dt 13 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp13.csv
wait
./my_dt 14 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp14.csv
wait
./my_dt 15 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp15.csv
wait
./my_dt 16 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp16.csv
wait
./my_dt 17 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp17.csv
wait
./my_dt 18 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp18.csv
wait
./my_dt 19 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp19.csv
wait
./my_dt 20 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp20.csv
wait
./my_dt 21 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp21.csv
wait
./my_dt 22 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp22.csv
wait
./my_dt 23 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp23.csv
wait
./my_dt 24 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp24.csv
wait
./my_dt 25 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp25.csv
wait
./my_dt 26 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp26.csv
wait
./my_dt 27 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp27.csv
wait
./my_dt 28 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp28.csv
wait
./my_dt 29 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp29.csv
wait
./my_dt 30 data/train.names data/train_corrected.csv data/test_corrected.csv data/dc_29_tree_dp30.csv
wait
#./my_dt 5 data/rekomendasi.names data/rekomendasi.csv data/rekomendasi.csv data/rekomendasi_tree.csv