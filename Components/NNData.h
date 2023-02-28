#pragma once
#ifndef __SDN_LAB_NNDATA_H
    #define __SDN_LAB_NNDATA_H
    #define __SDN_LAB_NNDATA_LEN 20
    #include "Settings.h"
    #include "Mcs.h"
    #include "Station.h"
    #include "ns3/ns3-ai-dl.h"                  // include DL model
    namespace SdnLab{
        // store the data (following the order of time increase)
        struct NNFeature{
            double time[__SDN_LAB_NNDATA_LEN];              // real time point (starting at the begining of the simulation in NS3)
            double rxPower[__SDN_LAB_NNDATA_LEN];           // power in Watt
        };
        // store MCS and its activate time point (following the order of data rate increase)
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
            NNData (void) = delete;
            NNData (uint16_t id) : Ns3AIDL<NNFeature, NNPredicted, NNTarget> (id){
                SetCond (2, 0); // set the mod condition => version % 2 == 0
            }
            /**
             * set feature
             */
            // void SetFeature(Station & station){

            // }
        };
    }

#endif