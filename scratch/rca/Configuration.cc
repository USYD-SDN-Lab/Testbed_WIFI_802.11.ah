#include "Configuration.h"

Configuration::Configuration() {
    
}

Configuration::Configuration(int argc, char *argv[]) {
    CommandLine cmd;
    /*cmd.AddValue("useIpv6", "Use Ipv6 (true/false)", useV6);
    cmd.AddValue("nControlLoops", "Number of control loops. If -1 all the stations will be in the loops if NSta is even", nControlLoops);
    cmd.AddValue("CoapPayloadSize", "Size of CoAP payload",coapPayloadSize);
     */
    /** self-defined parameters **/
    // projectname
    cmd.AddValue("projectname", "projectname", projectname);     // bound - rectangular

    // location
    cmd.AddValue("isLocRectangular", "isLocRectangular", isLocRectangular);     // bound - rectangular
    cmd.AddValue("isLocCircle", "isLocCircle", isLocCircle);                    
    cmd.AddValue("rho", "maximal distance between AP and stations", rho);
    cmd.AddValue("isLocRandom", "isLocRandom", isLocRandom);                    // initial loc - random
    cmd.AddValue("isLocUniform", "isLocUniform", isLocUniform);                 // inital loc - uniform
    cmd.AddValue("locUniformX", "locUniformX", locUniformX);                    // loc uniform - x
    cmd.AddValue("locUniformY", "locUniformY", locUniformY);                    // loc uniform - y
    // mobility
    cmd.AddValue("isMobStatic", "isMobStatic", isMobStatic);
    cmd.AddValue("isMobConstant", "isMobConstant", isMobConstant);
    cmd.AddValue("isMobRandomWaypoint", "isMobRandomWaypoint", isMobRandomWaypoint);
    cmd.AddValue("isMobRandomWalk", "isMobRandomWalk", isMobRandomWalk);
    cmd.AddValue("speedHoldTime", "speedHoldTime", speedHoldTime);
    cmd.AddValue("speedMin", "speedMin", speedMin);
    cmd.AddValue("speedMax", "speedMax", speedMax);
    cmd.AddValue("speedConstantX", "speedConstantX", speedConstantX);
    cmd.AddValue("speedConstantY", "speedConstantY", speedConstantY);

    cmd.AddValue("seed", "random seed", seed);
    cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
    cmd.AddValue("Nsta", "number of total stations", Nsta);
    cmd.AddValue("NRawSta", "number of stations supporting RAW. If -1 it will be based on NSta, should be divisible by NGroup", NRawSta);
    cmd.AddValue ("payloadSize", "Size of payload to send in bytes", payloadSize);
    cmd.AddValue("BeaconInterval", "Beacon interval time in us", BeaconInterval);
    cmd.AddValue("DataMode", "Date mode (check MCStoWifiMode for more details) (format: MCSbw_mcs, e.g. MCS1_0 is OfdmRate300KbpsBW1Mhz)", DataMode);
    cmd.AddValue("datarate", "data rate in Mbps", datarate);
    cmd.AddValue("bandWidth", "bandwidth in MHz", bandWidth);
    
    cmd.AddValue ("folder", "folder where result files are placed", folder);
    cmd.AddValue ("file", "files containing reslut information", file);
    cmd.AddValue ("totaltraffic", "totaltraffic", totaltraffic);
    cmd.AddValue ("TrafficPath", "files path of traffic file", TrafficPath);
    cmd.AddValue ("S1g1MfieldEnabled", "S1g1MfieldEnabled", S1g1MfieldEnabled);
    cmd.AddValue ("RAWConfigFile", "RAW Config file Path", RAWConfigFile);
    cmd.AddValue("TrafficType", "Kind of traffic (udp, -udpecho, -tcpecho, tcpipcamera, -tcpfirmware, -tcpsensor, -coap)", trafficType);
    cmd.AddValue("NGroup", "number of RAW groups", NGroup);
    cmd.AddValue("NRawSlotNum", "number of slots per RAW", NRawSlotNum);
    cmd.AddValue("pagePeriod", "Number of Beacon Intervals between DTIM beacons that carry Page Slice element for the associated page", pagePeriod);
    cmd.AddValue("pageSliceLength", "Number of blocks in each TIM for the associated page except for the last TIM (1-31)", pageSliceLength);
    cmd.AddValue("pageSliceCount", "Number of TIMs in a single page period (0-31; value 0 for whole page to be encoded in a single (only) TIM)", pageSliceCount);
    cmd.AddValue("blockOffset", "The 1st page slice starts with the block with blockOffset", blockOffset);
    cmd.AddValue("timOffset", "Offset in number of Beacon Intervals from the DTIM that carries the first page slice of the page", timOffset);
    cmd.AddValue("Outputpath", "files path of each stations", OutputPath);
    // parse input
    cmd.Parse(argc, argv);
    // format input
    /** self-defined parameters **/
	// location
    if(!isLocRectangular && !isLocCircle){
        NS_ABORT_MSG("The location boundary type is not assigned.");
    }
    if(!isLocRandom && !isLocUniform){
        NS_ABORT_MSG("The location intialization type is not assigned.");
    }
    // mobility
    if(!isMobStatic && !isMobConstant && !isMobRandomWaypoint && !isMobRandomWalk){
        NS_ABORT_MSG("The mobility type is not assigned.");
    }
    if(isMobConstant){
        if(speedConstantX == 0 && speedConstantY == 0){
            NS_ABORT_MSG("The mobility type (constant) must have a speed.");
        }
    }
    if(isMobRandomWalk){
        if(!isLocRectangular){
            NS_ABORT_MSG("The mobility type (random walk) must be in a rectangualr.");
        }
        if(speedHoldTime <= 0){
            NS_ABORT_MSG("The mobility type (random walk) must have positive [speedHoldTime].");
        }
    }
}
