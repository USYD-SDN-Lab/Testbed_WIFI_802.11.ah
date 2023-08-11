#!/bin/sh
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
projectname="STA128_C00_"$rangetype"_static"

for seed in $(seq 5 7)
do
    clear
    ./waf --run "rca --isRAaarf --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobStatic"
    clear
    ./waf --run "rca --isRAamrr --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobStatic"
    clear
    ./waf --run "rca --isRAMinstrel --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobStatic"
    clear
    ./waf --run "rca --isRAMinstrel --raMinstrelLookAroundRate=25 --projectname=$curprojectname --seed=$seed --simulationTime=$simtime --payloadSize=$packsize --RAWConfigFile=$RAWConfigFile --TrafficPath=$TrafficPath --BeaconInterval=$beacontime --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --rho=250 --isLocRandom --isMobStatic"
done