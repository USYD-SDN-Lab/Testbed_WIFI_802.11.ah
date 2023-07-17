# config (using Minstrel to generate RSSI & MCS data)
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_DEBUG -D__SDN_LAB_PHY_PACKET_SIZE_DATA=166 -D__SDN_LAB_PHY_PACKET_SIZE_BEACON=71" ./waf configure --disable-examples --disable-tests

# compile
./waf

# generate data
# human
projectname="nn_data_human"
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=5 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.0025 --speedMin=1.2 --speedMax=1.8"
done
# vehicle
projectname="nn_data_vehicle"
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=5 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.0025 --speedMin=8.3333 --speedMax=41.667"
done
# uav
projectname="nn_data_vehicle"
for seed in $(seq 5 7)
do
    ./waf --run "rca --seed=5 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text' --projectname=$projectname --isLocRectangular --isLocRandom --isMobRandomWalk --speedHoldTime=0.0025 --speedMin=83.3333 --speedMax=127.778"
done