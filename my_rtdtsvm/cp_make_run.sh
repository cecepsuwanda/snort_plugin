#!/bin/bash

cd src
cp -rf ~/snort_plugin/my_rtdtsvm/src/*.* .

cd ..
cd build-files
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..

cd ..
cmake --build ./build-files --target my_rtdtsvm -- -j 4


cd build-files/src
cp -rf ~/cpp/my_dtsvm/model .

rm -rf output/7week_friday.csv
./my_rtdtsvm /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/seventh_week/friday/outside.tcpdump output/7week_friday.csv model/model1
wait

rm -rf output/7week_monday.csv
./my_rtdtsvm /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/seventh_week/monday/tcpdump output/7week_monday.csv model/model1
wait

rm -rf output/7week_thursday.csv
./my_rtdtsvm /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/seventh_week/thursday/tcpdump output/7week_thursday.csv model/model1
wait

rm -rf output/7week_tuesday.csv
./my_rtdtsvm /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/seventh_week/tuesday/tcpdump output/7week_tuesday.csv model/model1
wait

rm -rf output/7week_wednesday.csv
./my_rtdtsvm /mnt/d/Thesis_Pasca_Unla/Dataset/mit_darpa_98/seventh_week/wednesday/tcpdump output/7week_wednesday.csv model/model1
wait

cd ../..