clear

# generate RSNN
projectname="test_rsnn"
CXXFLAGS="-std=c++11 -D__SDN_LAB_PRINT_NN_DATA_AVERAGE -D__SDN_LAB_DEBUG_NN -D__SDN_LAB_RA_MINSTREL_SNN_PLUS -D__SDN_LAB_DEBUG -D__SDN_LAB_PHY_PACKET_SIZE_DATA=166 -D__SDN_LAB_PHY_PACKET_SIZE_BEACON=71" ./waf configure --disable-examples --disable-tests
./waf
./waf --run "rca --seed=5 --projectname=$projectname --isRAMinstrel --simulationTime=110 --payloadSize=100 --BeaconInterval=500000 --pagePeriod=1 --pageSliceLength=1 --pageSliceCount=0 --isLocRectangular --rho=250 --isLocRandom --isMobRandomWalk --speedHoldTime=2 --speedMin=1.2 --speedMax=1.8"