#!/bin/sh
clear

# 00 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_00\"'

CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests

# 04 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_04\"'

# 08 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_08\"'

# 16 contention
projectname='\"Vincent_Scenario_Moving_Bad2Good_0m-100m_Contention_16\"'