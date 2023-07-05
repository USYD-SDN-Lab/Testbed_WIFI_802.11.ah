/*
 * Copyright (c) SDN Lab
 * 
 * <COMMENTS>
 * This header lists supported rate adaption algorithms 
 */
#pragma once
#ifndef __SDN_LAB_MODELDATALINK_H
    #define __SDN_LAB_MODELDATALINK_H
    #include <ns3/pointer.h>
    #include <ns3/double.h>
    #include <ns3/string.h>
    #include <ns3/wifi-helper.h>
    #include <ns3/yans-wifi-helper.h>

    // macros
    // macros - data adaption
    #define __SDN_LAB_MODELDATALINK_RA_CONSTANT                 0
    #define __SDN_LAB_MODELDATALINK_RA_AMRR                     1
    #define __SDN_LAB_MODELDATALINK_RA_AARF                     2
    #define __SDN_LAB_MODELDATALINK_RA_MINSTREL                 3
    #define __SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN             4
    #define __SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN_VINCENT     5
    #define __SDN_LAB_MODELDATALINK_RA_MINSTREL_RSNN            6

    using namespace std;
    using namespace ns3;

    namespace SdnLab{
        class ModelDataLink{
            protected:
                // RA
                unsigned int typeRA = __SDN_LAB_MODELDATALINK_RA_CONSTANT;
                // RA - Constant
                string raConstantDataRate;
                // RA - Minstrel
                double raMinstrelLookAroundRate = 10;

            public:
                /**
                 * Set RA algorithm type
                 * @isType:             whether we truely want to set RA to this type
                 * @raMinstrelLookAroundRate:     minstrel look around rate
                 */
                void RAType2Constant(string raConstantDataRate){this->typeRA =__SDN_LAB_MODELDATALINK_RA_CONSTANT;this->raConstantDataRate=raConstantDataRate;}
                void RAType2AMRR(){this->typeRA=__SDN_LAB_MODELDATALINK_RA_AMRR;}
                void RAType2AARF(){this->typeRA=__SDN_LAB_MODELDATALINK_RA_AARF;}
                void RAType2Minstrel(double raMinstrelLookAroundRate){this->typeRA=__SDN_LAB_MODELDATALINK_RA_MINSTREL;this->raMinstrelLookAroundRate=raMinstrelLookAroundRate;}
                void RAType2MinstrelSNN(double raMinstrelLookAroundRate){this->typeRA=__SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN;this->raMinstrelLookAroundRate=raMinstrelLookAroundRate;}
                void RAType2MinstrelSNNVincent(double raMinstrelLookAroundRate){this->typeRA=__SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN_VINCENT;this->raMinstrelLookAroundRate=raMinstrelLookAroundRate;}
                void RAType2MinstrelRSNN(double raMinstrelLookAroundRate){this->typeRA=__SDN_LAB_MODELDATALINK_RA_MINSTREL_RSNN;this->raMinstrelLookAroundRate=raMinstrelLookAroundRate;}


                /**
                 * SetRA  
                 */
                void SetRA(WifiHelper &wifi){
                    switch(this->typeRA){
                        case __SDN_LAB_MODELDATALINK_RA_CONSTANT:
                            wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue(this->raConstantDataRate), "ControlMode", StringValue(this->raConstantDataRate));
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_AMRR:
                            wifi.SetRemoteStationManager("ns3::AmrrWifiManager");
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_AARF:
                            wifi.SetRemoteStationManager("ns3::AarfWifiManager");
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL:
                            wifi.SetRemoteStationManager("ns3::MinstrelWifiManager", "LookAroundRate", DoubleValue(this->raMinstrelLookAroundRate));
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN:
                            wifi.SetRemoteStationManager("ns3::MinstrelWifiManager", "LookAroundRate", DoubleValue(this->raMinstrelLookAroundRate));
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN_VINCENT:
                            wifi.SetRemoteStationManager("ns3::MinstrelWifiManager", "LookAroundRate", DoubleValue(this->raMinstrelLookAroundRate));
                            cout<<this->raMinstrelLookAroundRate<<endl;
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL_RSNN:
                            wifi.SetRemoteStationManager("ns3::MinstrelWifiManager", "LookAroundRate", DoubleValue(this->raMinstrelLookAroundRate));
                            break;
                    }
                }


                /* OUTPUT RELATED */
                /**
                 * filename - throughput
                 */
                string FilenameThroughput(unsigned int seed){
                    string filename;
                    switch(this->typeRA){
                        case __SDN_LAB_MODELDATALINK_RA_CONSTANT:
                            filename = "throughput_const_rate_" + this->raConstantDataRate + "_" + to_string(seed) + ".csv";
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_AMRR:
                            filename = "throughput_amrr" + to_string(seed) + ".csv";
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_AARF:
                            filename = "throughput_aarf" + to_string(seed) + ".csv";
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL:
                            filename = "throughput_minstrel_p" + to_string(this->raMinstrelLookAroundRate) + "_" + to_string(seed) + ".csv";
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN:
                            filename = "throughput_minstrel_snn_p" + to_string(this->raMinstrelLookAroundRate) + "_" + to_string(seed) + ".csv";
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL_SNN_VINCENT:
                            filename = "throughput_minstrel_snn_vincent_p" + to_string(this->raMinstrelLookAroundRate) + "_" + to_string(seed) + ".csv";
                            break;
                        case __SDN_LAB_MODELDATALINK_RA_MINSTREL_RSNN:
                            filename = "throughput_minstrel_rsnn_p" + to_string(this->raMinstrelLookAroundRate) + "_" + to_string(seed) + ".csv";
                            break;
                    }
                }
        };
    }
#endif