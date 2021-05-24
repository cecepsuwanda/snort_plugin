#!/bin/bash

# DIR='Dataset/mit_darpa_98/train'
DIR='Dataset/mit_darpa_98/test'
# WEEKS=('1week' '2week' '3week' '4week' '5week' '6week' '7week')
WEEKS=('1week' '2week')
# WEEKS_STR=('first_week' 'second_week' 'third_week' 'fourth_week' 'fifth_week' 'sixth_week' 'seventh_week')
WEEKS_STR=('first_week' 'second_week')
DAYS=('monday' 'tuesday' 'wednesday' 'thursday' 'friday')

cd build-files/src
# rm -rf model
# mkdir model
# cp -rf ~/snort_plugin/my_rtdtsvm/model/model_100_700_svm_29attr/model* .

mkdir output1

COUNTER=0
for WEEK in ${WEEKS_STR[@]}; do
  for DAY in ${DAYS[@]}; do
   	rm -rf output1/${WEEKS[$COUNTER]}_$DAY.csv
   	./my_rtdtsvm ~/$DIR/$WEEK/$DAY/tcpdump output1/${WEEKS[$COUNTER]}_$DAY.csv model model1
   	done   	
  ((COUNTER++)) 	
done


# rm -rf output/1week_monday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/first_week/monday/tcpdump output/1week_monday.csv model model1
# wait

# rm -rf output/1week_thursday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/first_week/thursday/tcpdump output/1week_thursday.csv model model1
# wait

# rm -rf output/1week_tuesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/first_week/tuesday/tcpdump output/1week_tuesday.csv model model1
# wait

# rm -rf output/1week_wednesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/first_week/wednesday/tcpdump output/1week_wednesday.csv model model1
# wait

# rm -rf output/1week_friday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/first_week/friday/tcpdump output/1week_friday.csv model model1
# wait


# rm -rf output/2week_monday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/second_week/monday/tcpdump output/2week_monday.csv model model1
# wait

# rm -rf output/2week_thursday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/second_week/thursday/tcpdump output/2week_thursday.csv model model1
# wait

# rm -rf output/2week_tuesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/second_week/tuesday/tcpdump output/2week_tuesday.csv model model1
# wait

# rm -rf output/2week_wednesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/second_week/wednesday/tcpdump output/2week_wednesday.csv model model1
# wait

# rm -rf output/2week_friday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/second_week/friday/tcpdump output/2week_friday.csv model model1
# wait


# rm -rf output/3week_monday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/third_week/monday/tcpdump output/3week_monday.csv model model1
# wait

# rm -rf output/3week_thursday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/third_week/thursday/tcpdump output/3week_thursday.csv model model1
# wait

# rm -rf output/3week_tuesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/third_week/tuesday/tcpdump output/3week_tuesday.csv model model1
# wait

# rm -rf output/3week_wednesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/third_week/wednesday/tcpdump output/3week_wednesday.csv model model1
# wait

# rm -rf output/3week_friday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/third_week/friday/tcpdump output/3week_friday.csv model model1
# wait


# rm -rf output/4week_monday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fourth_week/monday/tcpdump output/4week_monday.csv model model1
# wait

# rm -rf output/4week_thursday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fourth_week/thursday/tcpdump output/4week_thursday.csv model model1
# wait

# rm -rf output/4week_tuesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fourth_week/tuesday/tcpdump output/4week_tuesday.csv model model1
# wait

# rm -rf output/4week_wednesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fourth_week/wednesday/tcpdump output/4week_wednesday.csv model model1
# wait

# rm -rf output/4week_friday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fourth_week/friday/tcpdump output/4week_friday.csv model model1
# wait


# rm -rf output/5week_monday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fifth_week/monday/tcpdump output/5week_monday.csv model model1
# wait

# rm -rf output/5week_thursday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fifth_week/thursday/tcpdump output/5week_thursday.csv model model1
# wait

# rm -rf output/5week_tuesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fifth_week/tuesday/tcpdump output/5week_tuesday.csv model model1
# wait

# rm -rf output/5week_wednesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fifth_week/wednesday/tcpdump output/5week_wednesday.csv model model1
# wait

# rm -rf output/5week_friday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/fifth_week/friday/tcpdump output/5week_friday.csv model model1
# wait


# rm -rf output/6week_monday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/sixth_week/monday/tcpdump output/6week_monday.csv model model1
# wait

# rm -rf output/6week_thursday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/sixth_week/thursday/tcpdump output/6week_thursday.csv model model1
# wait

# rm -rf output/6week_tuesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/sixth_week/tuesday/tcpdump output/6week_tuesday.csv model model1
# wait

# rm -rf output/6week_wednesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/sixth_week/wednesday/tcpdump output/6week_wednesday.csv model model1
# wait

# rm -rf output/6week_friday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/sixth_week/friday/tcpdump output/6week_friday.csv model model1
# wait


# rm -rf output/7week_monday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/seventh_week/monday/tcpdump output/7week_monday.csv model model1
# wait

# rm -rf output/7week_thursday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/seventh_week/thursday/tcpdump output/7week_thursday.csv model model1
# wait

# rm -rf output/7week_tuesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/seventh_week/tuesday/tcpdump output/7week_tuesday.csv model model1
# wait

# rm -rf output/7week_wednesday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/seventh_week/wednesday/tcpdump output/7week_wednesday.csv model model1
# wait

# rm -rf output/7week_friday.csv
# ./my_rtdtsvm ~/Dataset/mit_darpa_98/train/seventh_week/friday/tcpdump output/7week_friday.csv model model1
# wait

rm -rf ~/$DIR/hsl_extract
mkdir ~/$DIR/hsl_extract
cp -rf output1/*.csv ~/$DIR/hsl_extract

cd ../..