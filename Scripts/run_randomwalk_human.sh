#!/bin/sh
clear

projectname="Vincent_Scenario_Static_Contention_00_human_"
for speedHoldTime in 0.0025 0.05 0.1 0.2
do
    curprojectname="$projectname$speedHoldTime"
    # AMRR
    CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AMRR" ./waf configure --disable-examples --disable-tests
    ./waf
    for seed in $(seq 5 7)
    do
        ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=1.2 --speedMax=1.8"
    done
    
    # AARF
    CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AARF" ./waf configure --disable-examples --disable-tests
    ./waf
    for seed in $(seq 5 7)
    do
        ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=1.2 --speedMax=1.8"
    done

    # Minstrel - P10
    CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL" ./waf configure --disable-examples --disable-tests
    ./waf
    for seed in $(seq 5 7)
    do
        ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=1.2 --speedMax=1.8"
    done

    # Minstrel - P25
    CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
    ./waf
    for seed in $(seq 5 7)
    do
        ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=1.2 --speedMax=1.8"
    done

    # Minstrel-SNN (Vincent)
    CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
    ./waf
    for seed in $(seq 5 7)
    do
        ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=1.2 --speedMax=1.8"
    done

done