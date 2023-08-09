#!/bin/bash
clear

# compile
CXXFLAGS="-std=c++11" ./waf configure --disable-examples --disable-tests
# build
./waf

# other settings
simtime=110
packsize=100
beacontime=500000
RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt'
TrafficPath='./Components/Settings-Vincent-128-Traffic.text'
rangetype="rec"
projectname="C00_"$rangetype"_"

for vessel in 'human' 'vehicle' 'uav'
do
    # set speed
    speedmin=0
    speedmax=0
    if [ "$vessel" == "human" ]; then
        speedmin=1.2
        speedmax=1.8
    fi
    if [ "$vessel" == "vehicle" ]; then
        speedmin=8.3333
        speedmax=41.667
    fi
    if [ "$vessel" == "uav" ]; then
        speedmin=83.3333
        speedmax=127.778
    fi
    for speedHoldTime in 1 2 3 4 5
    do
        # set projectname
        curprojectname=$projectname$vessel"_"$speedHoldTime
        for seed in $(seq 5 7)
        do
            clear
            ./waf --run "rca --isRAaarf --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
            ./waf --run "rca --isRAamrr --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
            ./waf --run "rca --isRAMinstrel --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
            ./waf --run "rca --isRAMinstrel --raMinstrelLookAroundRate=25 --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
        done
    done
done