#!/bin/sh
clear


projectname="Vincent_Scenario_Static_Contention_00_vehicle_"
speedHoldTime=0.0025

#CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_PHY_PACKET_SIZE_DATA=166 -D__SDN_LAB_PHY_PACKET_SIZE_BEACON=71" ./waf configure --disable-examples --disable-tests
CXXFLAGS="-std=c++11" ./waf configure --disable-examples --disable-tests
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT" ./waf configure --disable-examples --disable-tests
./waf
curprojectname="$projectname$speedHoldTime"
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --isRAoptimal --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=8.3333 --speedMax=41.667"
done

#
#./waf --run "rca --seed=1 --DataMode='MCS4_9' --isRAoptimal --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.2 --speedMin=8.3333 --speedMax=41.667"

#./waf --run "rca --seed=1 --isRAaarf --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=1 --speedMin=83.3333 --speedMax=127.778 --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt'"
./waf --run "rca --seed=5 --projectname='Optimal-42' --isRAconstant --DataMode='MCS4_2' --simulationTime=100 --payloadSize=100 --rho=250 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --isLocUniform --locUniformX=191.2989 --isMobStatic"

#./waf --run "rca --seed=1 --raMinstrelLookAroundRate=25 --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.2 --speedMin=8.3333 --speedMax=41.667"
#./waf --run "rca --seed=1 --isRAMinstrel --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.2 --speedMin=8.3333 --speedMax=41.667"
#./waf --run "rca --seed=1 --isRAamrr --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.2 --speedMin=8.3333 --speedMax=41.667"

# projectname="Vincent_Scenario_Static_Contention_00_"
# for speedHoldTime in 0.0025 0.05 0.1 0.2
# do
#     curprojectname="$projectname$speedHoldTime"
#     # AMRR
#     CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AMRR" ./waf configure --disable-examples --disable-tests
#     ./waf
#     for seed in $(seq 5 7)
#     do
#         ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=8.3333 --speedMax=41.667"
#     done
    
#     # AARF
#     CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_AARF" ./waf configure --disable-examples --disable-tests
#     ./waf
#     for seed in $(seq 5 7)
#     do
#         ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=8.3333 --speedMax=41.667"
#     done

#     # Minstrel - P10
#     CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL" ./waf configure --disable-examples --disable-tests
#     ./waf
#     for seed in $(seq 5 7)
#     do
#         ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=8.3333 --speedMax=41.667"
#     done

#     # Minstrel - P25
#     CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
#     ./waf
#     for seed in $(seq 5 7)
#     do
#         ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=8.3333 --speedMax=41.667"
#     done

#     # Minstrel-SNN (Vincent)
#     CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
#     ./waf
#     for seed in $(seq 5 7)
#     do
#         ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$curprojectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=$speedHoldTime --speedMin=8.3333 --speedMax=41.667"
#     done

# done