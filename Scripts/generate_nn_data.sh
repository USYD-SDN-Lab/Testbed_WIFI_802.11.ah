#!/bin/bash
clear

# config
CXXFLAGS="-std=c++11" ./waf configure --disable-examples --disable-tests
# compile
./waf
clear

# other settings
macAddrShift=224
rho=250
simtime=240
packsize=100
beacontime=500000
pagePeriod=4
pageSliceLength=4
pageSliceCount=4
RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt'
TrafficPath='./Components/Settings-Vincent-128-Traffic.text'
rangetype="rec"
projectname="NNData_STA128_C00_"$rangetype"_"

# other settings (only for test)
# macAddrShift=300
# rho=250
# simtime=2
# packsize=100
# beacontime=102400
# pagePeriod=1
# pageSliceLength=1
# pageSliceCount=0
# RAWConfigFile='./OptimalRawGroup/RawConfig-rca.txt'
# TrafficPath='./OptimalRawGroup/traffic/data-1-1.0.txt'
# rangetype="rec"
# projectname="NNData_STA128_C00_"$rangetype"_"

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
        for seed in $(seq 5 7)
        do
            # calculate the shift
            curMacAddrShift=$((($seed-5)*$macAddrShift))
            ./waf --run "rca --ccMacAPLogRec --ccMacAPLogRecMacAddrShift=$curMacAddrShift --isRAMinstrel --raMinstrelLookAroundRate=25 --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=$pagePeriod --pageSliceLength=$pageSliceLength --pageSliceCount=$pageSliceCount --isLocRectangular --rho=$rho --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
        done
    done
done