# debug commands
#./waf --run "rca --ccMacAPLogRec --isRAMinstrel --raMinstrelLookAroundRate=25 --simulationTime=2 --pagePeriod=1 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=5 --speedMin=1.2 --speedMax=1.8"
#!/bin/bash
clear
# config
CXXFLAGS="-std=c++11" ./waf configure --disable-examples --disable-tests
# compile
./waf
clear

# settings - unchanged
rangetype="rec"
projectname="NNData_STA128_C00_"$rangetype"_"
rho=250

# settings - formal
seed_min=1
seed_max=10
macAddrShift=224
simtime=240
packsize=100
beacontime=500000
pagePeriod=4
pageSliceLength=4
pageSliceCount=4
RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt'
TrafficPath='./Components/Settings-Vincent-128-Traffic.text'

# change settings based on input
# set maCAddrShift
if [ "$1" != "1" ]; then
    macAddrShift=0
fi

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
    for speedHoldTime in $(seq 3 5)
    do
        curprojectname=$projectname$vessel"_"$speedHoldTime
        for seed in $(seq $seed_min $seed_max)
        do
            # calculate the shift
            curMacAddrShift=$((($seed-$seed_min)*$macAddrShift))
            ./waf --run "rca --ccMacAPLogRec --ccMacAPLogRecMacAddrShift=$curMacAddrShift --isRAMinstrel --raMinstrelLookAroundRate=25 --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=$pagePeriod --pageSliceLength=$pageSliceLength --pageSliceCount=$pageSliceCount --isLocRectangular --rho=$rho --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=$speedmin --speedMax=$speedmax"
        done
    done
done