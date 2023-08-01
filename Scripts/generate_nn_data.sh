# rsnn data
# config
CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG_NN -D__SDN_LAB_DEBUG -D__SDN_LAB_PHY_PACKET_SIZE_DATA=166 -D__SDN_LAB_PHY_PACKET_SIZE_BEACON=71" ./waf configure --disable-examples --disable-tests

# compile
./waf

# generate data
# human
projectname="rsnn_data_human"
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=1 --payloadSize=100 --BeaconInterval=500000 --pagePeriod=1 --pageSliceLength=1 --pageSliceCount=0 --projectname=$projectname --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=2 --speedMin=1.2 --speedMax=1.8"
done
# vehicle
projectname="rsnn_data_vehicle"
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.0025 --speedMin=8.3333 --speedMax=41.667"
done
# uav
projectname="rsnn_data_uav"
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=$seed --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.0025 --speedMin=83.3333 --speedMax=127.778"
done