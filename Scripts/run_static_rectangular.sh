#!/bin/sh
clear

projectname="Vincent_Scenario_Static_Contention_00_static_rectangular"
# AMRR
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AMRR" ./waf configure --disable-examples --disable-tests
./waf
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobStatic"
done

# AARF
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AARF" ./waf configure --disable-examples --disable-tests
./waf
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobStatic"
done

# Minstrel - P10
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL" ./waf configure --disable-examples --disable-tests
./waf
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobStatic"
done

# Minstrel - P25
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
./waf
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobStatic"
done

# Minstrel-SNN (Vincent)
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
./waf
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobStatic"
done
