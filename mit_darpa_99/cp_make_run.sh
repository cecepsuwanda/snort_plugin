#!/bin/bash

cd ~/cpp/mit_darpa_99
cp -rf ~/snort_plugin/mit_darpa_99/makefile .
cp -rf ~/snort_plugin/mit_darpa_99/Tfield_filter_darpa.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tbase_dataframe.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tdataframe_label.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tdataframe_darpa.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tdataframe_darpa2.* .
cp -rf ~/snort_plugin/mit_darpa_99/global_func.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tip_port_holder.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tdatetime_holder.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tread_file.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tread_file_darpa.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tolah_label.* .
cp -rf ~/snort_plugin/mit_darpa_99/Tconf_metrix.* .
cp -rf ~/snort_plugin/mit_darpa_99/main.cc .
rm labeling
make


# rm -rf data/darpa99_4w_friday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_friday_inside.csv data/darpa99_4w_friday_in.csv
# wait


# rm -rf data/darpa99_4w_friday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_friday_outside.csv data/darpa99_4w_friday_out.csv
# wait


# rm -rf data/darpa99_4w_monday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_monday_inside.csv data/darpa99_4w_monday_in.csv
# wait


# rm -rf data/darpa99_4w_monday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_monday_outside.csv data/darpa99_4w_monday_out.csv
# wait


# rm -rf data/darpa99_4w_thursday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_thursday_inside.csv data/darpa99_4w_thursday_in.csv
# wait


# rm -rf data/darpa99_4w_thursday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_thrusday_outside.csv data/darpa99_4w_thursday_out.csv
# wait


# rm -rf data/darpa99_4w_tuesday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_tuesday_outside.csv data/darpa99_4w_tuesday_out.csv
# wait


# rm -rf data/darpa99_4w_wednesday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_wednesday_inside.csv data/darpa99_4w_wednesday_in.csv
# wait


# rm -rf data/darpa99_4w_wednesday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/4week_wednesday_outside.csv data/darpa99_4w_wednesday_out.csv
# wait


# rm -rf data/darpa99_5w_friday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_friday_inside.csv data/darpa99_5w_friday_in.csv
# wait


# rm -rf data/darpa99_5w_monday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_monday_inside.csv data/darpa99_5w_monday_in.csv
# wait


# rm -rf data/darpa99_5w_monday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_monday_outside.csv data/darpa99_5w_monday_out.csv
# wait


# rm -rf data/darpa99_5w_thursday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_thursday_inside.csv data/darpa99_5w_thursday_in.csv
# wait


# rm -rf data/darpa99_5w_thursday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_thrusday_outside.csv data/darpa99_5w_thursday_out.csv
# wait


# rm -rf data/darpa99_5w_tuesday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_tuesday_inside.csv data/darpa99_5w_tuesday_in.csv
# wait


# rm -rf data/darpa99_5w_tuesday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_tuesday_outside.csv data/darpa99_5w_tuesday_out.csv
# wait


# rm -rf data/darpa99_5w_wednesday_in.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_wednesday_inside.csv data/darpa99_5w_wednesday_in.csv
# wait

# rm -rf data/darpa99_5w_wednesday_out.csv
# ./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/5week_wednesday_outside.csv data/darpa99_5w_wednesday_out.csv


rm -rf data/darpa99_2w_friday_out.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_friday_outside.csv data/darpa99_2w_friday_out.csv
wait

rm -rf data/darpa99_2w_friday_in.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_friday_inside.csv data/darpa99_2w_friday_in.csv
wait


rm -rf data/darpa99_2w_monday_in.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_monday_inside.csv data/darpa99_2w_monday_in.csv
wait


rm -rf data/darpa99_2w_monday_out.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_monday_outside.csv data/darpa99_2w_monday_out.csv
wait


rm -rf data/darpa99_2w_thursday_in.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_thursday_inside.csv data/darpa99_2w_thursday_in.csv
wait


rm -rf data/darpa99_2w_thursday_out.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_thrusday_outside.csv data/darpa99_2w_thursday_out.csv
wait


rm -rf data/darpa99_2w_tuesday_in.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_tuesday_inside.csv data/darpa99_2w_tuesday_in.csv
wait

rm -rf data/darpa99_2w_tuesday_out.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_tuesday_outside.csv rm -rf data/darpa99_2w_tuesday_out.csv
wait


rm -rf data/darpa99_2w_wednesday_out.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_wednesday_outside.csv data/darpa99_2w_wednesday_out.csv
wait

rm -rf data/darpa99_2w_wednesday_in.csv
./labeling ~/Dataset/mit_darpa_99/master_identifications.list ~/Dataset/mit_darpa_99/week2_attack.list ~/cpp/my_rtdtsvm/build-files/src/output/2week_wednesday_inside.csv data/darpa99_2w_wednesday_in.csv

