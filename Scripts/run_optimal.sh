#!/bin/sh
clear
projectname="Optimal"

CXXFLAGS="-std=c++11" ./waf configure --disable-examples --disable-tests
./waf
clear

for mcs in 'MCS1_0' 'MCS1_1' 'MCS1_2' 'MCS1_3' 'MCS1_4' 'MCS1_5' 'MCS1_6' 'MCS1_7' 'MCS1_8' 'MCS1_9' 'MCS2_0' 'MCS2_1' 'MCS2_2' 'MCS2_3' 'MCS2_4' 'MCS2_5' 'MCS2_6' 'MCS2_7' 'MCS2_8' 'MCS4_0' 'MCS4_1' 'MCS4_2' 'MCS4_3' 'MCS4_4' 'MCS4_5' 'MCS4_6' 'MCS4_7' 'MCS4_8' 'MCS4_9'
do
    for seed in $(seq 5 7)
    do
        clear
        ./waf --run "rca --seed=$seed --projectname=$projectname --isRAconstant --DataMode=$mcs --simulationTime=100 --payloadSize=100 --rho=250 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --isLocUniform --locUniformX=191.2989 --isMobStatic"
    done
done



