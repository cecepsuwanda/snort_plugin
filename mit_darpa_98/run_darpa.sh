#!/bin/bash

DIR='Dataset/mit_darpa_98/train'
# DIR='Dataset/mit_darpa_98/test'
WEEKS=('1week' '2week' '3week' '4week' '5week' '6week' '7week')
# WEEKS=('1week' '2week')
WEEKS1=('1w' '2w' '3w' '4w' '5w' '6w' '7w')
# WEEKS1=('1w' '2w')
WEEKS_STR=('first_week' 'second_week' 'third_week' 'fourth_week' 'fifth_week' 'sixth_week' 'seventh_week')
# WEEKS_STR=('first_week' 'second_week')
DAYS=('monday' 'tuesday' 'wednesday' 'thursday' 'friday')

# dos2unix ~/$DIR/hsl_extract/*.csv

rm -rf data/*.csv

COUNTER=0
for WEEK in ${WEEKS_STR[@]}; do
  for DAY in ${DAYS[@]}; do
   	 rm -rf data/darpa98_${WEEKS1[$COUNTER]}_$DAY.csv
     ./labeling ~/$DIR/$WEEK/$DAY/tcpdump.list ~/$DIR/hsl_extract/${WEEKS[$COUNTER]}_$DAY.csv data/darpa98_${WEEKS1[$COUNTER]}_$DAY.csv
   	done   	
  ((COUNTER++)) 	
done

# rm -rf data/darpa98_1w_monday.csv
# ./labeling ~/Dataset/mit_darpa_98/first_week/monday/tcpdump.list ~/Dataset/mit_darpa_98/output/1week_monday.csv ~/Dataset/gureKddcup/Week1/Monday/gureKddcup-matched.list data/darpa98_1w_monday.csv

# rm -rf data/darpa98_1w_thursday.csv
# ./labeling ~/Dataset/mit_darpa_98/first_week/thursday/tcpdump.list ~/Dataset/mit_darpa_98/output/1week_thursday.csv ~/Dataset/gureKddcup/Week1/Thursday/gureKddcup-matched.list data/darpa98_1w_thursday.csv

# rm -rf data/darpa98_1w_tuesday.csv
# ./labeling ~/Dataset/mit_darpa_98/first_week/tuesday/tcpdump.list ~/Dataset/mit_darpa_98/output/1week_tuesday.csv ~/Dataset/gureKddcup/Week1/Tuesday/gureKddcup-matched.list data/darpa98_1w_tuesday.csv

# rm -rf data/darpa98_1w_wednesday.csv
# ./labeling ~/Dataset/mit_darpa_98/first_week/wednesday/tcpdump.list ~/Dataset/mit_darpa_98/output/1week_wednesday.csv ~/Dataset/gureKddcup/Week1/Wednesday/gureKddcup-matched.list data/darpa98_1w_wednesday.csv

# rm -rf data/darpa98_1w_friday.csv
# ./labeling ~/Dataset/mit_darpa_98/first_week/friday/tcpdump.list ~/Dataset/mit_darpa_98/output/1week_friday.csv ~/Dataset/gureKddcup/Week1/Friday/gureKddcup-matched.list data/darpa98_1w_friday.csv


# rm -rf data/darpa98_2w_monday.csv
# ./labeling ~/Dataset/mit_darpa_98/second_week/monday/tcpdump.list ~/Dataset/mit_darpa_98/output/2week_monday.csv ~/Dataset/gureKddcup/Week2/Monday/gureKddcup-matched.list data/darpa98_2w_monday.csv

# rm -rf data/darpa98_2w_thursday.csv
# ./labeling ~/Dataset/mit_darpa_98/second_week/thursday/tcpdump.list ~/Dataset/mit_darpa_98/output/2week_thursday.csv ~/Dataset/gureKddcup/Week2/Thursday/gureKddcup-matched.list data/darpa98_2w_thursday.csv

# rm -rf data/darpa98_2w_tuesday.csv
# ./labeling ~/Dataset/mit_darpa_98/second_week/tuesday/tcpdump.list ~/Dataset/mit_darpa_98/output/2week_tuesday.csv ~/Dataset/gureKddcup/Week2/Tuesday/gureKddcup-matched.list data/darpa98_2w_tuesday.csv

# rm -rf data/darpa98_2w_wednesday.csv
# ./labeling ~/Dataset/mit_darpa_98/second_week/wednesday/tcpdump.list ~/Dataset/mit_darpa_98/output/2week_wednesday.csv ~/Dataset/gureKddcup/Week2/Wednesday/gureKddcup-matched.list data/darpa98_2w_wednesday.csv

# rm -rf data/darpa98_2w_friday.csv
# ./labeling ~/Dataset/mit_darpa_98/second_week/friday/tcpdump.list ~/Dataset/mit_darpa_98/output/2week_friday.csv ~/Dataset/gureKddcup/Week2/Friday/gureKddcup-matched.list data/darpa98_2w_friday.csv


# rm -rf data/darpa98_3w_monday.csv
# ./labeling ~/Dataset/mit_darpa_98/third_week/monday/tcpdump.list ~/Dataset/mit_darpa_98/output/3week_monday.csv ~/Dataset/gureKddcup/Week3/Monday/gureKddcup-matched.list data/darpa98_3w_monday.csv

# rm -rf data/darpa98_3w_thursday.csv
# ./labeling ~/Dataset/mit_darpa_98/third_week/thursday/tcpdump.list ~/Dataset/mit_darpa_98/output/3week_thursday.csv ~/Dataset/gureKddcup/Week3/Thursday/gureKddcup-matched.list data/darpa98_3w_thursday.csv

# rm -rf data/darpa98_3w_tuesday.csv
# ./labeling ~/Dataset/mit_darpa_98/third_week/tuesday/tcpdump.list ~/Dataset/mit_darpa_98/output/3week_tuesday.csv ~/Dataset/gureKddcup/Week3/Tuesday/gureKddcup-matched.list data/darpa98_3w_tuesday.csv

# rm -rf data/darpa98_3w_wednesday.csv
# ./labeling ~/Dataset/mit_darpa_98/third_week/wednesday/tcpdump.list ~/Dataset/mit_darpa_98/output/3week_wednesday.csv ~/Dataset/gureKddcup/Week3/Wednesday/gureKddcup-matched.list data/darpa98_3w_wednesday.csv

# rm -rf data/darpa98_3w_friday.csv
# ./labeling ~/Dataset/mit_darpa_98/third_week/friday/tcpdump.list ~/Dataset/mit_darpa_98/output/3week_friday.csv ~/Dataset/gureKddcup/Week3/Friday/gureKddcup-matched.list data/darpa98_3w_friday.csv


# rm -rf data/darpa98_4w_thursday.csv
# ./labeling ~/Dataset/mit_darpa_98/fourth_week/thursday/tcpdump.list ~/Dataset/mit_darpa_98/output/4week_thursday.csv ~/Dataset/gureKddcup/Week4/Thursday/gureKddcup-matched.list data/darpa98_4w_thursday.csv

# rm -rf data/darpa98_4w_tuesday.csv
# ./labeling ~/Dataset/mit_darpa_98/fourth_week/tuesday/tcpdump.list ~/Dataset/mit_darpa_98/output/4week_tuesday.csv ~/Dataset/gureKddcup/Week4/Tuesday/gureKddcup-matched.list data/darpa98_4w_tuesday.csv

# rm -rf data/darpa98_4w_wednesday.csv
# ./labeling ~/Dataset/mit_darpa_98/fourth_week/wednesday/tcpdump.list ~/Dataset/mit_darpa_98/output/4week_wednesday.csv ~/Dataset/gureKddcup/Week4/Wednesday/gureKddcup-matched.list data/darpa98_4w_wednesday.csv

# rm -rf data/darpa98_4w_friday.csv
# ./labeling ~/Dataset/mit_darpa_98/fourth_week/friday/tcpdump.list ~/Dataset/mit_darpa_98/output/4week_friday.csv ~/Dataset/gureKddcup/Week4/Friday/gureKddcup-matched.list data/darpa98_4w_friday.csv


# rm -rf data/darpa98_5w_monday.csv
# ./labeling ~/Dataset/mit_darpa_98/fifth_week/monday/tcpdump.list ~/Dataset/mit_darpa_98/output/5week_monday.csv ~/Dataset/gureKddcup/Week5/Monday/gureKddcup-matched.list data/darpa98_5w_monday.csv

# rm -rf data/darpa98_5w_thursday.csv
# ./labeling ~/Dataset/mit_darpa_98/fifth_week/thursday/tcpdump.list ~/Dataset/mit_darpa_98/output/5week_thursday.csv ~/Dataset/gureKddcup/Week5/Thursday/gureKddcup-matched.list data/darpa98_5w_thursday.csv

# rm -rf data/darpa98_5w_tuesday.csv
# ./labeling ~/Dataset/mit_darpa_98/fifth_week/tuesday/tcpdump.list ~/Dataset/mit_darpa_98/output/5week_tuesday.csv ~/Dataset/gureKddcup/Week5/Tuesday/gureKddcup-matched.list data/darpa98_5w_tuesday.csv

# rm -rf data/darpa98_5w_wednesday.csv
# ./labeling ~/Dataset/mit_darpa_98/fifth_week/wednesday/tcpdump.list ~/Dataset/mit_darpa_98/output/5week_wednesday.csv ~/Dataset/gureKddcup/Week5/Wednesday/gureKddcup-matched.list data/darpa98_5w_wednesday.csv

# rm -rf data/darpa98_5w_friday.csv
# ./labeling ~/Dataset/mit_darpa_98/fifth_week/friday/tcpdump.list ~/Dataset/mit_darpa_98/output/5week_friday.csv ~/Dataset/gureKddcup/Week5/Friday/gureKddcup-matched.list data/darpa98_5w_friday.csv


# rm -rf data/darpa98_6w_monday.csv
# ./labeling ~/Dataset/mit_darpa_98/sixth_week/monday/tcpdump.list ~/Dataset/mit_darpa_98/output/6week_monday.csv ~/Dataset/gureKddcup/Week6/Monday/gureKddcup-matched.list data/darpa98_6w_monday.csv

# rm -rf data/darpa98_6w_thursday.csv
# ./labeling ~/Dataset/mit_darpa_98/sixth_week/thursday/tcpdump.list ~/Dataset/mit_darpa_98/output/6week_thursday.csv ~/Dataset/gureKddcup/Week6/Thursday/gureKddcup-matched.list data/darpa98_6w_thursday.csv

# rm -rf data/darpa98_6w_tuesday.csv
# ./labeling ~/Dataset/mit_darpa_98/sixth_week/tuesday/tcpdump.list ~/Dataset/mit_darpa_98/output/6week_tuesday.csv ~/Dataset/gureKddcup/Week6/Tuesday/gureKddcup-matched.list data/darpa98_6w_tuesday.csv

# rm -rf data/darpa98_6w_wednesday.csv
# ./labeling ~/Dataset/mit_darpa_98/sixth_week/wednesday/tcpdump.list ~/Dataset/mit_darpa_98/output/6week_wednesday.csv ~/Dataset/gureKddcup/Week6/Wednesday/gureKddcup-matched.list data/darpa98_6w_wednesday.csv

# rm -rf data/darpa98_6w_friday.csv
# ./labeling ~/Dataset/mit_darpa_98/sixth_week/friday/tcpdump.list ~/Dataset/mit_darpa_98/output/6week_friday.csv ~/Dataset/gureKddcup/Week6/Friday/gureKddcup-matched.list data/darpa98_6w_friday.csv


# rm -rf data/darpa98_7w_monday.csv
# ./labeling ~/Dataset/mit_darpa_98/seventh_week/monday/tcpdump.list ~/Dataset/mit_darpa_98/output/7week_monday.csv ~/Dataset/gureKddcup/Week7/Monday/gureKddcup-matched.list data/darpa98_7w_monday.csv

# rm -rf data/darpa98_7w_thursday.csv
# ./labeling ~/Dataset/mit_darpa_98/seventh_week/thursday/tcpdump.list ~/Dataset/mit_darpa_98/output/7week_thursday.csv ~/Dataset/gureKddcup/Week7/Thursday/gureKddcup-matched.list data/darpa98_7w_thursday.csv

# rm -rf data/darpa98_7w_tuesday.csv
# ./labeling ~/Dataset/mit_darpa_98/seventh_week/tuesday/tcpdump.list ~/Dataset/mit_darpa_98/output/7week_tuesday.csv ~/Dataset/gureKddcup/Week7/Tuesday/gureKddcup-matched.list data/darpa98_7w_tuesday.csv

# rm -rf data/darpa98_7w_wednesday.csv
# ./labeling ~/Dataset/mit_darpa_98/seventh_week/wednesday/tcpdump.list ~/Dataset/mit_darpa_98/output/7week_wednesday.csv ~/Dataset/gureKddcup/Week7/Wednesday/gureKddcup-matched.list data/darpa98_7w_wednesday.csv

# rm -rf data/darpa98_7w_friday.csv
# ./labeling ~/Dataset/mit_darpa_98/seventh_week/friday/tcpdump.list ~/Dataset/mit_darpa_98/output/7week_friday.csv ~/Dataset/gureKddcup/Week7/Friday/gureKddcup-matched.list data/darpa98_7w_friday.csv

rm -rf ~/$DIR/label
mkdir ~/$DIR/label
cp -rf data/*.csv ~/$DIR/label

