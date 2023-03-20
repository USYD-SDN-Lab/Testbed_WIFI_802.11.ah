#pragma once
#ifndef __SDN_LAB_NNDATA_H
    #define __SDN_LAB_NNDATA_H
    // C/C++ & python shared settings
    #define __SDN_LAB_NNDATA_MEMORY_ID      7418        // the memory pool id accross python and C/C++
    #define __SDN_LAB_NNDATA_LEN            20
    #define __SDN_LAB_MCS_NUM               29          // supported MCS number
    #define __SDN_LAB_NNDATA_ILLEGAL_DATA   0
    #include "ns3/ns3-ai-dl.h"                          // include DL model
    #include "Settings.h"
    #include "Station.h"
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
            /**
             * get the predicted
             * @mcsList:                the pointer to a mcs list
             * @mcsActivateTimeList:    the pointer to the activation time of each mcs
             * @listMaxLen:             the maximal length of these two lists (that share the same length)
             */
            void GetPredicted(unsigned int * mcsList, double * mcsActivateTimeList, unsigned int listMaxLen){
                // report error in case of memory shortage
                if (listMaxLen < __SDN_LAB_MCS_NUM){
                    Toolbox::Error err("/Components", "NNData.h", "NNData", "GetPredicted", "actual data outstrips the memory size");
                    err.SetType2MemoryShortage();
                    throw err;
                }
                // operate when the pointer is not null
                if(mcsList && mcsActivateTimeList){
                    unsigned int i;
                    // append data
                    auto pred = PredictedGetterCond();
                    for(i = 0; i < __SDN_LAB_MCS_NUM; ++i){
                        mcsList[i] = pred->mcs[i];
                        mcsActivateTimeList[i] = pred->mcsActivateTime[i];
                    }
                    GetCompleted();
                    // set the rest to 0
                    for(i = __SDN_LAB_MCS_NUM - 1; i < listMaxLen; ++i){
                        mcsList[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                        mcsActivateTimeList[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                    }
                }
            }
            unsigned int GetPredicted(){
                auto pred = PredictedGetterCond();
                unsigned int mcs = pred->mcs[0];
                GetCompleted();
                return mcs;
            }
        };
    }

#endif