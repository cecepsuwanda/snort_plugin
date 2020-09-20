#!/bin/bash

cd ~/cpp/mit_darpa_99
cp -rf /mnt/e/snort_plugin/mit_darpa_99/makefile .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/Tread_file.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/Tolah_label.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/Tconf_metrix.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/main.cc .
make

rm /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
./labeling data/master_identifications.list ../my_dt/data/4w_friday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_friday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_monday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_monday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_thursday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_thursday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_tuesday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_wednesday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/4w_wednesday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
# ./labeling data/master_identifications.list ../my_dt/data/4w_monday_41122222.csv 
# wait
# ./labeling data/master_identifications.list ../my_dt/data/4w_monday_41084031.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/4w_monday_41114554.csv

# wait
# ./labeling data/master_identifications.list ../my_dt/data/4w_monday_41213446_in.csv

# wait
# ./labeling data/master_identifications.list ../my_dt/data/4w_monday_41213446_out.csv

# wait
# ./labeling data/master_identifications.list ../my_dt/data/4w_monday_41213446_out_icmp.csv

# wait
# ./labeling data/master_identifications.list ../my_dt/data/4w_monday_41161308_in.csv


./labeling data/master_identifications.list ../my_dt/data/5w_friday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_monday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_monday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_thursday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_thursday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_tuesday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_tuesday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_wednesday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
wait
./labeling data/master_identifications.list ../my_dt/data/5w_wednesday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv

# ./labeling data/master_identifications.list ../my_dt/data/2w_friday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_friday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_monday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_monday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_thursday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_thursday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_tuesday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_tuesday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_wednesday_out.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv
# wait
# ./labeling data/master_identifications.list ../my_dt/data/2w_wednesday_in.csv /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv

sudo chmod 777 /home/cecep/python/mit_darpa_99/data/mit_darpa_99_label.csv