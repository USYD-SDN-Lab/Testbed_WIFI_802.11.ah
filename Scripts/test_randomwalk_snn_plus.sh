#!/bin/bash
clear

# compile
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_PLUS" ./waf configure --disable-examples --disable-tests
# build
./waf

# other settings
simtime=104
packsize=100
rangetype="rec"
projectname="Test_C00_"$rangetype"_"

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
    for speedHoldTime in 2 3 4 5
    do
        # set projectname
        curprojectname=$projectname$vessel"_"$speedHoldTime
        for seed in $(seq 5 7)
        do
            clear
            ./waf --run "rca --raMinstrelLookAroundRate=25 --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --pagePeriod=1 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
        done
    done
done