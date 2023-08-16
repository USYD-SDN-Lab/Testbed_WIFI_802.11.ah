#!/bin/bash
clear

# config
CXXFLAGS="-std=c++11" ./waf configure --disable-examples --disable-tests
# compile
./waf
clear

# other settings
simtime=240
packsize=100
beacontime=500000
RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt'
TrafficPath='./Components/Settings-Vincent-128-Traffic.text'
rangetype="rec"
projectname="NNData_STA128_C00_"$rangetype"_"

# generate data
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
    for speedHoldTime in $(seq 1 5)
    do
        curprojectname=$projectname$vessel"_"$speedHoldTime
        # 3*1200 = 3600(s) data
        for seed in $(seq 5 7)
        do
            ./waf --run "rca --ccMacAPLogRec --isRAMinstrel --raMinstrelLookAroundRate=25 --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
        done
    done
done