#!/bin/sh
clear

# AMRR
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AMRR -D__SDN_LAB_PROJECTNAME='\"Vincent_Scenario_RWM_Contention_04\"'" ./waf configure --disable-examples --disable-tests
# AARF
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AARF -D__SDN_LAB_PROJECTNAME='\"Vincent_Scenario_RWM_Contention_04\"'" ./waf configure --disable-examples --disable-tests
# Minstrel - P10
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_PROJECTNAME='\"Vincent_Scenario_RWM_Contention_04\"'" ./waf configure --disable-examples --disable-tests
# Minstrel - P25
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25 -D__SDN_LAB_PROJECTNAME='\"Vincent_Scenario_RWM_Contention_04\"'" ./waf configure --disable-examples --disable-tests
# Minstrel-SNN (Vincent)
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25 -D__SDN_LAB_PROJECTNAME='\"Vincent_Scenario_RWM_Contention_04\"'" ./waf configure --disable-examples --disable-tests


./waf --run "rca --seed=5 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-04-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"
./waf --run "rca --seed=6 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-04-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"
./waf --run "rca --seed=7 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-04-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"