#pragma once
#ifndef __SDN_LAB_NNDATA_H
    #define __SDN_LAB_NNDATA_H
    // C/C++ & python shared settings
    #define __SDN_LAB_NNDATA_MEMORY_ID      7418        // the memory pool id accross python and C/C++
    #define __SDN_LAB_NNDATA_LEN            20
    #define __SDN_LAB_MCS_NUM               29          // supported MCS number
    #define __SDN_LAB_NNDATA_ILLEGAL_DATA   0
    #include "ns3/ns3-ai-dl.h"                          // include DL model
    #include "Components/Settings.h"
    #include "Components/Station.h"
    #include "Modules/Toolbox/Error.h"                  // Error to throw
    namespace SdnLab{
        // store the data (following the order of time increase)
        // for a single STA
        // memory pool size taken at (8+8+4)*20 = 400 bytes
        struct NNFeature{
            // general
            double time[__SDN_LAB_NNDATA_LEN];              // real time point (starting at the begining of the simulation in NS3)
            double rxPower[__SDN_LAB_NNDATA_LEN];           // power in Watt
            unsigned int bandwidth[__SDN_LAB_NNDATA_LEN];   // bandwidth
        };
        // store MCS and its activate time point (following the order of data rate increase)
        // for a single STA
        // memory pool size taken at (4+8)*20 = 240 bytes
        struct NNPredicted{
            // general
            unsigned int mcs[__SDN_LAB_MCS_NUM];            // a low index means a low data rate
            double mcsActivateTime[__SDN_LAB_MCS_NUM];      // relative time point (starting at 0)
        };
        // not used
        struct NNTarget{
            uint8_t target;
        };

        // the memory struct shared between C/C++ and Python
        class NNData: public ns3::Ns3AIDL<NNFeature, NNPredicted, NNTarget>{
        public:
            /**
             * constructor 
             */
            NNData (void) : Ns3AIDL<NNFeature, NNPredicted, NNTarget> (__SDN_LAB_NNDATA_MEMORY_ID){
                SetCond (2, 0); // set the mod condition => version % 2 == 0 (even in C/C++, odd in Python)
            }
            /**
             * set feature
             * @station: the reference of a station
             */
            void SetFeatures(Station & station){
                auto feature = FeatureSetterCond();
                station.GetTimeList(feature->time, __SDN_LAB_NNDATA_LEN);
                station.GetRxPowerList(feature->rxPower, __SDN_LAB_NNDATA_LEN);
                station.GetBandwidthList(feature->bandwidth, __SDN_LAB_NNDATA_LEN);
                SetCompleted();
            }
            void SetFeatures(Station * station){
                if(station){
                    SetFeatures(*station);
                }
            }
            /**
             * get all predicts
             * @station: the station to accept predicts
             */
            void GetPredicts(Station & station){
                auto pred = PredictedGetterCond();
                station.SetNNData(pred->mcs, pred->mcsActivateTime, __SDN_LAB_NNDATA_LEN);
                printf("GetPredicts");
                for(int i = 0; i < __SDN_LAB_NNDATA_LEN; ++i){
                    printf("%d, ", pred->mcs[i]);
                }
                printf("\n");
                GetCompleted();
            }
            void GetPredicts(Station * station){
                if(station){
                    GetPredicts(*station);
                }
            }
        };
    }

#endif