#pragma once
#ifndef __PACKETCONTEXT_H
    #define __PACKETCONTEXT_H
    #include "ns3/mac48-address.h"      // support Mac48Address

    class PacketContext{
        private:
        /*** mac layer (of a MPDU) ***/
        uint32_t macPacketSize;         // the packet size (MAC)
        ns3::Mac48Address txMacAddr;    // Tx MAC address
        ns3::Mac48Address rxMacAddr;    // Rx MAC address
        /*** physical layer (contains an AMPDU of several MPDU or only a MPDU ) ***/
        uint32_t phyPacketSize;         // the packet size (Physical)
        double startTime;               // packet start time (sec)
        double endTime;                 // packet end time (sec)
        uint32_t bandwidth;             // the bandwidth (Hz)
        unsigned int mcs_in;            // the comming MCS (self-defined)
        unsigned int mcs_predict;       // the predicted MCS (self-defined)
        double per;                     // the probability error rate of the packet
        double snr;                     // SNR
        double rxPower;                 // Rx power in Watt
        double interferePower;          // the interference power
        bool isReceived = false;        // whether this packet is received
        
        /*** Constructor & Deconstructor ***/
        PacketContext(uint32_t packetSize, double startTime, double endTime, uint32_t bandwidth, unsigned int mcs_in, double per, double snr, double rxPower, double interferePower){
            this->phyPacketSize = packetSize;
            this->startTime = startTime;
            this->endTime = endTime;
            this->bandwidth = bandwidth;
            this->mcs_in = mcs_in;
            this->per = per;
            this->snr = snr;
            this->rxPower = rxPower;
            this->interferePower = interferePower;
        };
        ~PacketContext(){};

        public:
        /**
         * retrieve the bandwidth from the mode name
         */
        static uint32_t ModeName2Bandwidth(std::string modeName){
            uint32_t bandwidth = 0;
            if (modeName == "OfdmRate300KbpsBW1MHz" ||
                modeName == "OfdmRate600KbpsBW1MHz" ||
                modeName == "OfdmRate900KbpsBW1MHz" ||
                modeName == "OfdmRate1_2MbpsBW1MHz" ||
                modeName == "OfdmRate1_8MbpsBW1MHz" ||
                modeName == "OfdmRate2_4MbpsBW1MHz" ||
                modeName == "OfdmRate2_7MbpsBW1MHz" ||
                modeName == "OfdmRate3MbpsBW1MHz"   ||
                modeName == "OfdmRate3_6MbpsBW1MHz" ||
                modeName == "OfdmRate4MbpsBW1MHz"   ||
                modeName == "OfdmRate150KbpsBW1MHz"){
                bandwidth = 1;
            }else if (modeName == "OfdmRate650KbpsBW2MHz"  ||
                      modeName == "OfdmRate1_3MbpsBW2MHz"  ||
                      modeName == "OfdmRate1_95MbpsBW2MHz" ||
                      modeName == "OfdmRate2_6MbpsBW2MHz"  ||
                      modeName == "OfdmRate3_9MbpsBW2MHz"  ||
                      modeName == "OfdmRate5_2MbpsBW2MHz"  ||
                      modeName == "OfdmRate5_85MbpsBW2MHz" ||
                      modeName == "OfdmRate6_5MbpsBW2MHz"  ||
                      modeName == "OfdmRate7_8MbpsBW2MHz"){
                bandwidth = 2;
            }else if (modeName == "OfdmRate1_35MbpsBW4MHz" ||
                      modeName == "OfdmRate2_7MbpsBW4MHz"  ||
                      modeName == "OfdmRate4_05MbpsBW4MHz" ||
                      modeName == "OfdmRate5_4MbpsBW4MHz"  ||
                      modeName == "OfdmRate8_1MbpsBW4MHz"  ||
                      modeName == "OfdmRate10_8MbpsBW4MHz" ||
                      modeName == "OfdmRate12_15MbpsBW4MHz"||
                      modeName == "OfdmRate13_5MbpsBW4MHz" ||
                      modeName == "OfdmRate16_2MbpsBW4MHz" ||
                      modeName == "OfdmRate18MbpsBW4MHz"){
                bandwidth = 4;
            }
            return bandwidth;
        };
        /**
         * retrieve MCS from the mode name
         */
        static unsigned int ModeName2MCS(std::string modeName){
            unsigned int mcs = 0;
            if (modeName == "OfdmRate300KbpsBW1MHz"){
                mcs = 10;
            }else if(modeName == "OfdmRate600KbpsBW1MHz"){
                mcs = 11;
            }else if(modeName == "OfdmRate900KbpsBW1MHz"){
                mcs = 12;
            }else if(modeName == "OfdmRate1_2MbpsBW1MHz"){
                mcs = 13;
            }else if(modeName == "OfdmRate1_8MbpsBW1MHz"){
                mcs = 14;
            }else if(modeName == "OfdmRate2_4MbpsBW1MHz"){
                mcs = 15;
            }else if(modeName == "OfdmRate2_7MbpsBW1MHz"){
                mcs = 16;
            }else if(modeName == "OfdmRate3MbpsBW1MHz"){
                mcs = 17;
            }else if(modeName == "OfdmRate3_6MbpsBW1MHz"){
                mcs = 18;
            }else if(modeName == "OfdmRate4MbpsBW1MHz"){
                mcs = 19;
            }else if(modeName == "OfdmRate150KbpsBW1MHz"){
                mcs = 110;
            }else if(modeName == "OfdmRate650KbpsBW2MHz"){
                mcs = 20;
            }else if(modeName == "OfdmRate1_3MbpsBW2MHz"){
                mcs = 21;
            }else if(modeName == "OfdmRate1_95MbpsBW2MHz"){
                mcs = 22;
            }else if(modeName == "OfdmRate2_6MbpsBW2MHz"){
                mcs = 23;
            }else if(modeName == "OfdmRate3_9MbpsBW2MHz"){
                mcs = 24;
            }else if(modeName == "OfdmRate5_2MbpsBW2MHz"){
                mcs = 25;
            }else if(modeName == "OfdmRate5_85MbpsBW2MHz"){
                mcs = 26;
            }else if(modeName == "OfdmRate6_5MbpsBW2MHz"){
                mcs = 27;
            }else if(modeName == "OfdmRate7_8MbpsBW2MHz"){
                mcs = 28;
            }else if(modeName == "OfdmRate1_35MbpsBW4MHz"){
                mcs = 40;
            }else if(modeName == "OfdmRate2_7MbpsBW4MHz"){
                mcs = 41;
            }else if(modeName == "OfdmRate4_05MbpsBW4MHz"){
                mcs = 42;
            }else if(modeName == "OfdmRate5_4MbpsBW4MHz"){
                mcs = 43;
            }else if(modeName == "OfdmRate8_1MbpsBW4MHz"){
                mcs = 44;
            }else if(modeName == "OfdmRate10_8MbpsBW4MHz"){
                mcs = 45;
            }else if(modeName == "OfdmRate12_15MbpsBW4MHz"){
                mcs = 46;
            }else if(modeName == "OfdmRate13_5MbpsBW4MHz"){
                mcs = 47;
            }else if(modeName == "OfdmRate16_2MbpsBW4MHz"){
                mcs = 48;
            }else if(modeName == "OfdmRate18MbpsBW4MHz"){
                mcs = 49;
            }
            return mcs;
        };
        /**
         * create a PacketContext
         * <INPUT>
         * @startTime:                  packet start time (sec)
         * @endTime:                    packet end time (sec)
         * @per:
         * @snr:
         * @rxPower:
         * @interferePower:
         * @modeName:                   the mode to calculate bandwidth & MCS
         */
        static PacketContext * Create(uint32_t packetSize, double startTime, double endTime, double per, double snr, double rxPower, double interferePower, std::string modeName){
            // calculate bandwidth & mcs_in from the mode name
            uint32_t bandwidth = PacketContext::ModeName2Bandwidth(modeName);
            unsigned int mcs_in = PacketContext::ModeName2MCS(modeName);
            // return
            return new PacketContext(packetSize, startTime, endTime, bandwidth, mcs_in, per, snr, rxPower, interferePower);
        }
        /**
         * destory a PacketContext
         */
        static void Destory(const PacketContext * ptrPacketContext){
            if(ptrPacketContext){
                delete ptrPacketContext;
            }
        }
        
        /*** Get & Set ***/
        // Tx & Rx Mac address
        void SetTxRxMacAddr(ns3::Mac48Address txMacAddr, ns3::Mac48Address rxMacAddr){
            this->txMacAddr = txMacAddr;
            this->rxMacAddr = rxMacAddr;
        }
        // Mac packet size  
        void SetMacPacketSize(uint32_t packetSize){
            this->macPacketSize = packetSize;
        }
        // Phy packet size
        uint32_t GetPhyPacketSize(){
            return this->phyPacketSize;
        }
        // startTime
        double GetStartTime(){
            return this->startTime;
        }
        // end time
        double GetEndTime(){
            return this->endTime;
        }
        // is received
        void SetReceived(){
            this->isReceived = true;
        }
        void SetNotReceived(){
            this->isReceived = false;
        }
        bool IsReceived(){
            return this->isReceived;
        }
        // MCS - Coming
        unsigned int GetMCSIn(){
            return this->mcs_in;
        }
        // MCS - Predicted
        void SetMCSPredict(unsigned int mcs_predict){
            this->mcs_predict = mcs_predict;
        }
        unsigned int GetMCSPredict(){
            return this->mcs_predict;
        }
    };
    /*** redefined other relevant type names ***/
    typedef PacketContext * PtrPacketContext;
#endif