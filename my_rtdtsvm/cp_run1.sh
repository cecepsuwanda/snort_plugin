#!/bin/bash

cd build-files/src
rm -rf model
mkdir model
cp -rf ~/cpp/my_dtsvm/model .

mkdir output

rm -rf output/2week_monday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/monday/inside.tcpdump output/2week_monday_inside.csv model/model1
wait

rm -rf output/2week_monday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/monday/outside.tcpdump output/2week_monday_outside.csv model/model1
wait

rm -rf output/2week_thursday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/thursday/inside.tcpdump output/2week_thursday_inside.csv model/model1
wait

rm -rf output/2week_thursday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/thursday/outside.tcpdump output/2week_thrusday_outside.csv model/model1
wait

rm -rf output/2week_tuesday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/tuesday/inside.tcpdump output/2week_tuesday_inside.csv model/model1
wait

rm -rf output/2week_tuesday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/tuesday/outside.tcpdump output/2week_tuesday_outside.csv model/model1
wait

rm -rf output/2week_wednesday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/wednesday/inside.tcpdump output/2week_wednesday_inside.csv model/model1
wait

rm -rf output/2week_wednesday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/wednesday/outside.tcpdump output/2week_wednesday_outside.csv model/model1
wait

rm -rf output/2week_friday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/friday/inside.tcpdump output/2week_friday_inside.csv model/model1
wait

rm -rf output/2week_friday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/second_week/friday/outside.tcpdump output/2week_friday_outside.csv model/model1
wait


rm -rf output/4week_monday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/monday/inside.tcpdump output/4week_monday_inside.csv model/model1
wait

rm -rf output/4week_monday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/monday/outside.tcpdump output/4week_monday_outside.csv model/model1
wait

rm -rf output/4week_thursday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/thursday/inside.tcpdump output/4week_thursday_inside.csv model/model1
wait

rm -rf output/4week_thursday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/thursday/outside.tcpdump output/4week_thrusday_outside.csv model/model1
wait

rm -rf output/4week_tuesday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/tuesday/outside.tcpdump output/4week_tuesday_outside.csv model/model1
wait

rm -rf output/4week_wednesday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/wednesday/inside.tcpdump output/4week_wednesday_inside.csv model/model1
wait

rm -rf output/4week_wednesday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/wednesday/outside.tcpdump output/4week_wednesday_outside.csv model/model1
wait

rm -rf output/4week_friday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/friday/inside.tcpdump output/4week_friday_inside.csv model/model1
wait

rm -rf output/4week_friday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fourth_week/friday/outside.tcpdump output/4week_friday_outside.csv model/model1
wait


rm -rf output/5week_monday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/monday/inside.tcpdump output/5week_monday_inside.csv model/model1
wait

rm -rf output/5week_monday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/monday/outside.tcpdump output/5week_monday_outside.csv model/model1
wait

rm -rf output/5week_thursday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/thursday/inside.tcpdump output/5week_thursday_inside.csv model/model1
wait

rm -rf output/5week_thursday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/thursday/outside.tcpdump output/5week_thrusday_outside.csv model/model1
wait

rm -rf output/5week_tuesday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/tuesday/inside.tcpdump output/5week_tuesday_inside.csv model/model1
wait

rm -rf output/5week_tuesday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/tuesday/outside.tcpdump output/5week_tuesday_outside.csv model/model1
wait

rm -rf output/5week_wednesday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/wednesday/inside.tcpdump output/5week_wednesday_inside.csv model/model1
wait

rm -rf output/5week_wednesday_outside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/wednesday/outside.tcpdump output/5week_wednesday_outside.csv model/model1
wait

rm -rf output/5week_friday_inside.csv
./my_rtdtsvm ~/Dataset/mit_darpa_99/fifth_week/friday/inside.tcpdump output/5week_friday_inside.csv model/model1
wait

cd ../..