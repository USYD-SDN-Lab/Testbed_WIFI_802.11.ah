#pragma once
#ifndef __SDN_LAB_NNDATA_H
    #define __SDN_LAB_NNDATA_H
    // C/C++ & python shared settings
    #define __SDN_LAB_NNDATA_MEMORY_ID      7418        // the memory pool id accross python and C/C++
    #define __SDN_LAB_NNDATA_ILLEGAL_DATA   0
    #include "ns3/ns3-ai-dl.h"                          // include DL model
    #include "ns3/mac48-address.h"                      // support Mac48Address
    #include "Components/Settings.h"
    #include "Components/Station.h"
    #include "Modules/Toolbox/Error.h"                  // Error to throw
    namespace SdnLab{
        // store the data (following the order of time increase)
        // for a single STA
        // memory pool size taken at (8+8+4)*20 = 400 bytes
        struct NNFeature{
            uint8_t mac[6];             // mac address
            double time;                // real time point (starting at the begining of the simulation in NS3)
            double rxPower;             // power in Watt
        };
        // store MCS and its activate time point (following the order of data rate increase)
        // for a single STA
        // memory pool size taken at (4+8)*20 = 240 bytes
        struct NNPredicted{
            // general
            unsigned int mcs;            // a low index means a low data rate
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
             */
            void SetFeatures(ns3::Mac48Address macAddr, double time, double rxPower){
                auto feature = FeatureSetterCond();
                macAddr.CopyTo(feature->mac);
                feature->time = time;
                feature->rxPower = rxPower;
                SetCompleted();
            }
            void SetFeatures(double time){
                auto feature = FeatureSetterCond();
                for(int i = 0; i <6; i++)
                    feature->mac[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                feature->time = time;
                feature->rxPower = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                SetCompleted();
            }
            /**
             * get all predicts
             * @station: the station to accept predicts
             */
            unsigned int  GetPredict(){
                auto pred = PredictedGetterCond();
                unsigned int mcs = pred->mcs;
                GetCompleted();
                return mcs;
            }
        };
    }

#endif