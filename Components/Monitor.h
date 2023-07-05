/*
 * Copyright (c) SDN Lab
 * 
 * <COMMENTS>
 * This file provides the monitoring of data:
 *  (1) 
 */
#pragma once
#ifndef __SDN_LAB_MONITOR_H
    #define __SDN_LAB_MONITOR_H
    
    using namespace std;

    namespace SdnLab{
        class Monitor{
            protected:
                void PrintThroughput(double pastTime, unsigned int pastSentPackets, unsigned int pastSuccessfulPackets){
                    // set the throughput file path based on the wifi manager
	string path = __SDN_LAB_SET_STATISTIC_PATH(settings, config);

	// record STA position
	string posFilePath = settings.PathProjectReport() + "position.csv";
	Ptr<MobilityModel> mob = wifiStaNode.Get(0)->GetObject<MobilityModel>();
    Vector pos = mob->GetPosition ();
	Vector velocity = mob->GetVelocity();
	double speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	if(fm.Open(posFilePath) == 200){
		fm.AddCSVItem(pos.x);
		fm.AddCSVItem(pos.y);
		fm.AddCSVItem(speed);
		fm.AddCSVItem(velocity.x);
		fm.AddCSVItem(velocity.y);
		fm.AddCSVItem(velocity.z, true);
		fm.Close();
	}
	
	// do next when the path is not empty
	if(!path.empty()){
		// get the current time
		double curTime = Simulator::Now().GetSeconds();
		// calculate statistics
		// calculate statistics - total
		unsigned int totalSentPackets = 0;
		unsigned int totalSuccessfulPackets = 0; 
		double totalThroughput = 0;
		for (int i = 0; i < config.Nsta; i++){
			totalSentPackets += stats.get(i).NumberOfSentPackets;
			totalSuccessfulPackets += stats.get(i).NumberOfSuccessfulPackets;
		}
		totalThroughput = totalSuccessfulPackets * config.payloadSize * 8 / (curTime * 1000000.0);
		// calculate statistics - transient
		unsigned int curSentPackets = totalSentPackets - pastSentPackets;
		unsigned int curSuccessfulPackets = totalSuccessfulPackets - pastSuccessfulPackets; 
		double curThroughput = curSuccessfulPackets * config.payloadSize * 8 / ((curTime - pastTime) * 1000000.0);
		// write to file
		if(fm.Open(path) == 200){
			fm.AddCSVItem(curTime);
			// additive
			fm.AddCSVItem(curSentPackets);
			fm.AddCSVItem(curSuccessfulPackets);
			fm.AddCSVItem(curThroughput);				// Mbits/s
			// total
			fm.AddCSVItem(totalSentPackets);
			fm.AddCSVItem(totalSuccessfulPackets);
			fm.AddCSVItem(totalThroughput, true);		// Mbits/s
			fm.Close();
		}
		// schedule the next
		Simulator::Schedule(Seconds(1), &PrintStatistics, curTime, totalSentPackets, totalSuccessfulPackets);
	}
                }
        };  
    }
#endif