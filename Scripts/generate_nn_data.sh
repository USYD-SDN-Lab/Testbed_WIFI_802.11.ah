# config (using Minstrel to generate RSSI & MCS data)
CXXFLAGS="-std=c++11 -D__SDN_LAB_RA_MINSTREL -D__SDN_LAB_PROJECTNAME='\"nn_data\"' -D__SDN_LAB_DEBUG -D__SDN_LAB_PHY_PACKET_SIZE_DATA=166 -D__SDN_LAB_PHY_PACKET_SIZE_BEACON=71" ./waf configure --disable-examples --disable-tests
# compile
./waf
# run
#./waf --run "rca --seed=5 --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./Components/Settings-Vincent-128-Contention-00-RawConfig.txt' --TrafficPath='./Components/Settings-Vincent-128-Traffic.text'"
./waf --run "rca --seed=1 --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"