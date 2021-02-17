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