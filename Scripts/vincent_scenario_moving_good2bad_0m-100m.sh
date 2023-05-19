#!/bin/sh
clear

# 00 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_00\"'

CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_PHY_PACKET_SIZE_DATA=166 -D__SDN_LAB_PHY_PACKET_SIZE_BEACON=71 -D__SDN_LAB_STA_LOC_CUSTOM -D__SDN_LAB_STA_LOC_X=50 -D__SDN_LAB_STA_LOC_Y=0 -D__SDN_LAB_MOB_MOVING -D__SDN_LAB_RA_MINSTREL" ./waf configure --disable-examples --disable-tests
./waf --run "rca --seed=1 --simulationTime=100 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"

# 04 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_04\"'

# 08 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_08\"'

# 16 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_16\"'