#!/bin/sh
clear
#############################################################
# static - 0 contention
#
# projectname='\"Vincent_Scenario_Static_Contention_00\"'
# # optimal rate
# CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_CONST_RATE -D__SDN_LAB_PROJECTNAME=$projectname" ./waf configure --disable-examples --disable-tests
# ./waf
# for mcs in 'MCS1_0' 'MCS1_1' 'MCS1_2' 'MCS1_3' 'MCS1_4' 'MCS1_5' 'MCS1_6' 'MCS1_7' 'MCS1_8' 'MCS1_9' 'MCS2_0' 'MCS2_1' 'MCS2_2' 'MCS2_3' 'MCS2_4' 'MCS2_5' 'MCS2_6' 'MCS2_7' 'MCS2_8' 'MCS4_0' 'MCS4_1' 'MCS4_2' 'MCS4_3' 'MCS4_4' 'MCS4_5' 'MCS4_6' 'MCS4_7' 'MCS4_8' 'MCS4_9' 'MCS1_5' 'MCS1_6' 'MCS1_7' 'MCS1_8' 'MCS1_9' 'MCS2_0' 'MCS2_1' 'MCS2_2' 'MCS2_3' 'MCS2_4' 'MCS2_5' 'MCS2_6' 'MCS2_7' 'MCS2_8' 'MCS4_0' 'MCS4_1' 'MCS4_2' 'MCS4_3' 'MCS4_4' 'MCS4_5' 'MCS4_6' 'MCS4_7' 'MCS4_8' 'MCS4_9'
# do
#     for seed in $(seq 5 7)
#     do
#         ./waf --run "rca --seed=$seed --DataMode=$mcs --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"
#     done
# done
#
#############################################################

#############################################################
# static - 4 contention
#
projectname='\"Vincent_Scenario_Static_Contention_04\"'
# optimal rate
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_CONST_RATE -D__SDN_LAB_PROJECTNAME=$projectname" ./waf configure --disable-examples --disable-tests
./waf
for mcs in 'MCS1_0' 'MCS1_1' 'MCS1_2' 'MCS1_3' 'MCS1_4' 'MCS1_5' 'MCS1_6' 'MCS1_7' 'MCS1_8' 'MCS1_9' 'MCS2_0' 'MCS2_1' 'MCS2_2' 'MCS2_3' 'MCS2_4' 'MCS2_5' 'MCS2_6' 'MCS2_7' 'MCS2_8' 'MCS4_0' 'MCS4_1' 'MCS4_2' 'MCS4_3' 'MCS4_4' 'MCS4_5' 'MCS4_6' 'MCS4_7' 'MCS4_8' 'MCS4_9' 'MCS1_5' 'MCS1_6' 'MCS1_7' 'MCS1_8' 'MCS1_9' 'MCS2_0' 'MCS2_1' 'MCS2_2' 'MCS2_3' 'MCS2_4' 'MCS2_5' 'MCS2_6' 'MCS2_7' 'MCS2_8' 'MCS4_0' 'MCS4_1' 'MCS4_2' 'MCS4_3' 'MCS4_4' 'MCS4_5' 'MCS4_6' 'MCS4_7' 'MCS4_8' 'MCS4_9'
do
    for seed in $(seq 5 7)
    do
        ./waf --run "rca --seed=$seed --DataMode=$mcs --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-04-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"
    done
done
#
#############################################################


# 0 contention

#./waf --run "rca --seed=6 --DataMode='MCS1_1' --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-0-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"
#./waf --run "rca --seed=7 --DataMode='MCS1_1' --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-0-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"



# AMRR
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AMRR" ./waf configure --disable-examples --disable-tests
# AARF
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AARF" ./waf configure --disable-examples --disable-tests
# Minstrel - P10
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL" ./waf configure --disable-examples --disable-tests
# Minstrel - P25
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
# Minstrel-SNN (Vincent)
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
# Minstrel-SNN
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN" ./waf configure --disable-examples --disable-tests
# Minstrel-SNN+
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_PLUS" ./waf configure --disable-examples --disable-tests
# Minstrel-AI-Dist
#CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_AI_DIST" ./waf configure --disable-examples --disable-tests


./waf --run "rca --seed=1 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-04-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"