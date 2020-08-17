#!/bin/bash

cd ~/cpp/mit_darpa_99
cp -rf /mnt/e/snort_plugin/mit_darpa_99/makefile .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/Tread_file.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/Tolah_label.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/Tconf_metrix.* .
cp -rf /mnt/e/snort_plugin/mit_darpa_99/main.cc .
make

./labeling