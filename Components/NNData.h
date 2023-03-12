#pragma once
#ifndef __SDN_LAB_NNDATA_H
    #define __SDN_LAB_NNDATA_H
    #define __SDN_LAB_NNDATA_MEMORY_ID 1234     // the memory pool id accross python and C/C++
    #define __SDN_LAB_NNDATA_LEN 20
    #define __SDN_LAB_NNDATA_ILLEGAL_DATA 0
    #include "ns3/ns3-ai-dl.h"                  // include DL model
    #include "Settings.h"
    #include "Mcs.h"
    #include "Station.h"
    #include "Modules/Toolbox/Error.h"          // Error to throw
    namespace SdnLab{
        // store the data (following the order of time increase)
        // for a single STA
        // memory pool size taken at (8+8)*30 = 480 bytes
        struct NNFeature{
            double time[__SDN_LAB_NNDATA_LEN];              // real time point (starting at the begining of the simulation in NS3)
            double rxPower[__SDN_LAB_NNDATA_LEN];           // power in Watt
        };
        // store MCS and its activate time point (following the order of data rate increase)
        // for a single STA
        // memory pool size taken at (4+8)*30 = 360 bytes
        struct NNPredicted{
            unsigned int mcs[__SDN_LAB_MCS_NUM];            // a low index means a low data rate
            double mcsActivateTime[__SDN_LAB_MCS_NUM];      // relative time point (starting at 0)
        };
        // not used
        struct NNTarget{
            uint8_t target;
        };

        // the memory struct shared between C/C++ and Python
        class NNData: public ns3::Ns3AIDL<NNFeature, NNPredicted, NNTarget>{
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
                    // init the list with 0
                    for(i = 0; i < listMaxLen; ++i){
                        mcsList[i] = 0;
                        mcsActivateTimeList[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                    }
                    // append data
                    auto pred = PredictedGetterCond();
                    for(i = 0; i < __SDN_LAB_MCS_NUM; ++i){
                        if(pred->mcs[i] == __SDN_LAB_NNDATA_ILLEGAL_DATA){
                            break;
                        }
                        mcsList[i] = pred->mcs[i];
                        mcsActivateTimeList[i] = pred->mcsActivateTime[i];
                    }
                    GetCompleted();
                }
            }
        };
    }

#endif