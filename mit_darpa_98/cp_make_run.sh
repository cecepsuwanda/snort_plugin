#!/bin/bash

cd ~/cpp/mit_darpa_98
cp -rf ~/snort_plugin/mit_darpa_98/makefile .
cp -rf ~/snort_plugin/mit_darpa_98/Tread_file.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tolah_label.* .
cp -rf ~/snort_plugin/mit_darpa_98/Tconf_metrix.* .
cp -rf ~/snort_plugin/mit_darpa_98/main.cc .
make

./labeling ~/Dataset/mit_darpa_98/snort_log/1w_monday.csv 1 Mon ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/1w_thursday.csv 1 Thurs ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/1w_tuesday.csv 1 Tues ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/1w_wednesday.csv 1 Wed ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
wait
./labeling ~/Dataset/mit_darpa_98/snort_log/1w_friday.csv 1 Fri ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
wait


# ./labeling ~/Dataset/mit_darpa_98/snort_log/2w_monday.csv 2 Mon ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/2w_thursday.csv 2 Thurs ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/2w_tuesday.csv 2 Tues ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/2w_wednesday.csv 2 Wed ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/2w_friday.csv 2 Fri ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait


# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_monday.csv 3 Mon ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_thursday.csv 3 Thurs ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_tuesday.csv 3 Tues ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_wednesday.csv 3 Wed ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/3w_friday.csv 3 Fri ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_thursday.csv 4 Thurs ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_tuesday.csv 4 Tues ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_wednesday.csv 4 Wed ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/4w_friday.csv 4 Fri ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_monday.csv 5 Mon ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_thursday.csv 5 Thurs ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_tuesday.csv 5 Tues ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_wednesday.csv 5 Wed ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/5w_friday.csv 5 Fri ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_monday.csv 6 Mon ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_thursday.csv 6 Thurs ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_tuesday.csv 6 Tues ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_wednesday.csv 6 Wed ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/6w_friday.csv 6 Fri ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait

# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_monday.csv 7 Mon ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_thursday.csv 7 Thurs ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_tuesday.csv 7 Tues ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_wednesday.csv 7 Wed ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ~/Dataset/mit_darpa_98/snort_log/7w_friday.csv 7 Fri ~/Dataset/mit_darpa_98/attack.csv ~/Dataset/mit_darpa_98/host_ip_addr.csv
# wait

