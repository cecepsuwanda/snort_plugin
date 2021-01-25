#!/bin/bash

cd ~/cpp/mit_darpa_98
cp -rf ~/snort_plugin/mit_darpa_98/makefile .
cp -rf ~/snort_plugin/mit_darpa_98/Tread_file.* .
cp -rf ~/snort_plugin/mit_darpa_98/global_func.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tbase_dataframe.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tip_port_holder.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tdatetime_holder.* .
cp -rf ~/snort_plugin/mit_darpa_98/Trange_holder.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tconf_metrix.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tdataframe_list.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tdataframe_label.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tdataframe_gure.* .
cp -rf ~/snort_plugin/mit_darpa_98/main.cc .
wait
rm labeling
wait
make



# ./labeling ~/Dataset/mit_darpa_98/seventh_week/monday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_monday.csv ~/Dataset/gureKddcup/Week7/Monday/gureKddcup-matched.list data/darpa98_7w_friday.csv

# ./labeling ~/Dataset/mit_darpa_98/seventh_week/thursday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_thursday.csv ~/Dataset/gureKddcup/Week7/Thursday/gureKddcup-matched.list data/darpa98_7w_friday.csv

# ./labeling ~/Dataset/mit_darpa_98/seventh_week/tuesday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_tuesday.csv ~/Dataset/gureKddcup/Week7/Tuesday/gureKddcup-matched.list data/darpa98_7w_friday.csv

# ./labeling ~/Dataset/mit_darpa_98/seventh_week/wednesday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_wednesday.csv ~/Dataset/gureKddcup/Week7/Wednesday/gureKddcup-matched.list data/darpa98_7w_friday.csv

./labeling ~/Dataset/mit_darpa_98/seventh_week/friday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_friday.csv ~/Dataset/gureKddcup/Week7/Friday/gureKddcup-matched.list data/darpa98_7w_friday.csv



