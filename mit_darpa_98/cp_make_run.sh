#!/bin/bash

cd ~/cpp/mit_darpa_98
cp -rf ~/snort_plugin/mit_darpa_98/makefile .
cp -rf ~/snort_plugin/mit_darpa_98/Tread_file.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tolah_label.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tconf_metrix.* .
cp -rf ~/snort_plugin/mit_darpa_98/main.cc .
make

rm ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv

# ./labeling ~/Dataset/mit_darpa_98/snort_log/1w_monday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/1w_thursday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/1w_tuesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/1w_wednesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/1w_friday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait


./labeling ~/Dataset/mit_darpa_98/snort_log/2w_monday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/2w_thursday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/2w_tuesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/2w_wednesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/2w_friday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
wait


# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_monday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_thursday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_tuesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_wednesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_friday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_thursday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_tuesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_wednesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_friday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_monday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_thursday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_tuesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_wednesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_friday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_monday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_thursday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_tuesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_wednesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_friday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_monday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_thursday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_tuesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_wednesday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_friday.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_friday_smurf.csv ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv ~/Dataset/mit_darpa_98/mit_darpa_98_label.csv
# wait

