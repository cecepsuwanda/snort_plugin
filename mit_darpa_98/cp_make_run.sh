#!/bin/bash

cd ~/cpp/mit_darpa_98
cp -rf /mnt/e/snort_plugin/mit_darpa_98/makefile .
cp -rf /mnt/e/snort_plugin/mit_darpa_98/Tread_file.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_98/Tolah_label.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_98/Tconf_metrix.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_98/main.cc .
make

# ./labeling ../my_dt/data/1w_monday.csv 1 Mon /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/attack.csv /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ../my_dt/data/1w_thursday.csv 1 Thurs /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/attack.csv /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
./labeling ../my_dt/data/1w_tuesday.csv 1 Tues /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/attack.csv /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ../my_dt/data/1w_wednesday.csv 1 Wed /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/attack.csv /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ../my_dt/data/1w_friday.csv 1 Fri /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/attack.csv /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/host_ip_addr.csv
# wait
# ./labeling ../my_dt/data/1w_monday.csv 1 Mon /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/attack.csv /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/host_ip_addr.csv
# wait