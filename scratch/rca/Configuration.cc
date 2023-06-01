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
    cmd.AddValue("locX", "location in X", locX);
    cmd.AddValue("locY", "location in Y", locY);
    cmd.AddValue("mobilitySpeedMin", "the minimal speed", mobilitySpeedMin);
    cmd.AddValue("mobilitySpeedMax", "the maximal speed", mobilitySpeedMax);
    cmd.AddValue("mobilityAngle", "the angle", mobilityAngle);
    cmd.AddValue("mobilityAcceleration", "the speed acceleration", mobilityAcceleration);
    cmd.AddValue("mobilityInterval", "the interval to change the speed", mobilityInterval);

    cmd.AddValue("seed", "random seed", seed);
    cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
    cmd.AddValue("Nsta", "number of total stations", Nsta);
    cmd.AddValue("NRawSta", "number of stations supporting RAW. If -1 it will be based on NSta, should be divisible by NGroup", NRawSta);
    cmd.AddValue ("payloadSize", "Size of payload to send in bytes", payloadSize);
    cmd.AddValue("BeaconInterval", "Beacon interval time in us", BeaconInterval);
    cmd.AddValue("DataMode", "Date mode (check MCStoWifiMode for more details) (format: MCSbw_mcs, e.g. MCS1_0 is OfdmRate300KbpsBW1Mhz)", DataMode);
    cmd.AddValue("datarate", "data rate in Mbps", datarate);
    cmd.AddValue("bandWidth", "bandwidth in MHz", bandWidth);
    cmd.AddValue("rho", "maximal distance between AP and stations", rho);
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
    this->mobilityAcceleration = abs(this->mobilityAcceleration);
}
