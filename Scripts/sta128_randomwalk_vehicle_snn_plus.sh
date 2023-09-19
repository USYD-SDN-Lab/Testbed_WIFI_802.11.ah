#!/bin/bash
clear

# compile
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT" ./waf configure --disable-examples --disable-tests
# build
./waf

# other settings
simtime=110
packsize=100
beacontime=500000
RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt'
TrafficPath='./Components/Settings-Vincent-128-Traffic.text'
rangetype="rec"
projectname="STA128_C00_"$rangetype"_"

for vessel in 'vehicle'
do
    # set speed
    speedmin=0
    speedmax=0
    if [ "$vessel" == "vehicle" ]; then
        speedmin=8.3333
        speedmax=41.667
    fi
    for speedHoldTime in 5
    do
        # set projectname
        curprojectname=$projectname$vessel"_"$speedHoldTime
        for seed in $(seq 5 7)
        do
            clear
            ./waf --run "rca --raMinstrelLookAroundRate=25 --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
        done
    done
done