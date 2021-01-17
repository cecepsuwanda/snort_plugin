#!/bin/bash

cd ~/cpp/mit_darpa_98
cp -rf ~/snort_plugin/mit_darpa_98/makefile .
cp -rf ~/snort_plugin/mit_darpa_98/Tread_file.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tolah_label.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tconf_metrix.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tdataframe_list.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tdataframe_label.* .
cp -rf ~/snort_plugin/mit_darpa_98/main.cc .
make


./labeling ~/Dataset/mit_darpa_98/seventh_week/monday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_monday.csv

./labeling ~/Dataset/mit_darpa_98/seventh_week/thursday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_thursday.csv

./labeling ~/Dataset/mit_darpa_98/seventh_week/tuesday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_tuesday.csv

./labeling ~/Dataset/mit_darpa_98/seventh_week/wednesday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_wednesday.csv

./labeling ~/Dataset/mit_darpa_98/seventh_week/friday/tcpdump.list ~/cpp/my_rtdtsvm/build-files/src/output/7week_friday.csv



