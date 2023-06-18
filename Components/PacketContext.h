#pragma once
#ifndef __SDN_LAB_PACKETCONTEXT_H
    #define __SDN_LAB_PACKETCONTEXT_H
    #include <stdlib.h>
    #include <time.h> 
    #include <iostream>
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "ns3/wifi-mac-header.h"    // support WifiMacHeader
    #include "Mcs.h"                    // MCS
    #include "Mac.h"                    // Mac constants
    #include "Overhead.h"               // overhead
    namespace SdnLab{
        class _PacketContext{
            private:
            /*** members ***/
            int id = 0;
            bool isEmpty = true;
            // mac layer (of a MPDU)
            uint32_t macPacketSize          = 0;         // the packet size (MAC)
            ns3::Mac48Address sourMacAddr   = __SDN_LAB_MAC_BROADCAST_ADDR;     // source MAC address
            ns3::Mac48Address destMacAddr   = __SDN_LAB_MAC_BROADCAST_ADDR;     // destination MAC address
            ns3::Mac48Address txMacAddr     = __SDN_LAB_MAC_BROADCAST_ADDR;     // Tx MAC address
            ns3::Mac48Address rxMacAddr     = __SDN_LAB_MAC_BROADCAST_ADDR;     // Rx MAC address
            ns3::Mac48Address bssid         = __SDN_LAB_MAC_BROADCAST_ADDR;     // basic service set identifier (MAC address)
            // physical layer (contains an AMPDU of several MPDU or only a MPDU )
            uint32_t phyPacketSize          = 0;                // the packet size (Physical)
            double startTime                = -1;               // packet start time (sec)
            double endTime                  = -1;               // packet end time (sec)
            uint32_t bandwidth              = 0;                // the bandwidth (Hz)
            unsigned int mcs_in             = 0;                // the comming MCS (self-defined)
            unsigned int mcs_predict        = 0;                // the predicted MCS (self-defined)
            double per                      = -1;               // the probability error rate of the packet
            double snr                      = -1;               // SNR
            double rxPower                  = -1;               // Rx power in Watt
            double interferePower           = -1;               // the interference power
            bool isReceived                 = false;            // whether this packet is received
            uint32_t nodeIndex              = 0xFFFFFFFF;       // the node (device) assigned to this packet
            // overhead
            Overhead * overhead = NULL;

            public:
            /*** Constructor & Deconstructor ***/
            _PacketContext(){
                srand (time(NULL));
                this->id = rand();
            };
            _PacketContext(uint32_t packetSize, double startTime, double endTime, double per, double snr, double rxPower, double interferePower):_PacketContext(){
                // not empty
                this->isEmpty = false;
                // set data
                this->phyPacketSize = packetSize;
                this->startTime = startTime;
                this->endTime = endTime;
                this->per = per;
                this->snr = snr;
                this->rxPower = rxPower;
                this->interferePower = interferePower;
            }
            _PacketContext(uint32_t packetSize, double startTime, double endTime, uint32_t bandwidth, unsigned int mcs_in, double per, double snr, double rxPower, double interferePower) : _PacketContext(packetSize, startTime, endTime, per, snr, rxPower, interferePower){                
                this->bandwidth = bandwidth;
                this->mcs_in = mcs_in;
            };
            _PacketContext(uint32_t packetSize, double startTime, double endTime, double per, double snr, double rxPower, double interferePower, std::string modeName) : _PacketContext(packetSize, startTime, endTime, per, snr, rxPower, interferePower){
                // calculate bandwidth & mcs_in from the mode name
                uint32_t bandwidth = _PacketContext::ModeName2Bandwidth(modeName);
                unsigned int mcs_in = _PacketContext::ModeName2MCS(modeName);
                // assign
                this->bandwidth = bandwidth;
                this->mcs_in = mcs_in;
            };
            ~_PacketContext(){
                Clear();
            };
            /**
             * Clear all allocated space
             */
            void Clear(){
                // set to empty
                this->isEmpty = true;
                // overhead
                Overhead::Destroy(this->overhead);
                this->overhead = NULL;
            }

            /**
             * retrieve the bandwidth from the mode name
             */
            static uint32_t ModeName2Bandwidth(std::string modeName){
                uint32_t bandwidth = 0;
                if (modeName == __SDN_LAB_MCS_10 ||
                    modeName == __SDN_LAB_MCS_11 ||
                    modeName == __SDN_LAB_MCS_12 ||
                    modeName == __SDN_LAB_MCS_13 ||
                    modeName == __SDN_LAB_MCS_14 ||
                    modeName == __SDN_LAB_MCS_15 ||
                    modeName == __SDN_LAB_MCS_16 ||
                    modeName == __SDN_LAB_MCS_17 ||
                    modeName == __SDN_LAB_MCS_18 ||
                    modeName == __SDN_LAB_MCS_19 ||
                    modeName == __SDN_LAB_MCS_110){
                    bandwidth = 1000000;
                }else if (modeName == __SDN_LAB_MCS_20 ||
                        modeName == __SDN_LAB_MCS_21 ||
                        modeName == __SDN_LAB_MCS_22 ||
                        modeName == __SDN_LAB_MCS_23 ||
                        modeName == __SDN_LAB_MCS_24 ||
                        modeName == __SDN_LAB_MCS_25 ||
                        modeName == __SDN_LAB_MCS_26 ||
                        modeName == __SDN_LAB_MCS_27 ||
                        modeName == __SDN_LAB_MCS_28){
                    bandwidth = 2000000;
                }else if (modeName == __SDN_LAB_MCS_40 ||
                        modeName == __SDN_LAB_MCS_41 ||
                        modeName == __SDN_LAB_MCS_42 ||
                        modeName == __SDN_LAB_MCS_43 ||
                        modeName == __SDN_LAB_MCS_44 ||
                        modeName == __SDN_LAB_MCS_45 ||
                        modeName == __SDN_LAB_MCS_46 ||
                        modeName == __SDN_LAB_MCS_47 ||
                        modeName == __SDN_LAB_MCS_48 ||
                        modeName == __SDN_LAB_MCS_49){
                    bandwidth = 4000000;
                }
                return bandwidth;
            };
            /**
             * retrieve MCS from the mode name
             */
            static unsigned int ModeName2MCS(std::string modeName){
                unsigned int mcs = 0;
                if (modeName == __SDN_LAB_MCS_10){
                    mcs = 10;
                }else if(modeName == __SDN_LAB_MCS_11){
                    mcs = 11;
                }else if(modeName == __SDN_LAB_MCS_12){
                    mcs = 12;
                }else if(modeName == __SDN_LAB_MCS_13){
                    mcs = 13;
                }else if(modeName == __SDN_LAB_MCS_14){
                    mcs = 14;
                }else if(modeName == __SDN_LAB_MCS_15){
                    mcs = 15;
                }else if(modeName == __SDN_LAB_MCS_16){
                    mcs = 16;
                }else if(modeName == __SDN_LAB_MCS_17){
                    mcs = 17;
                }else if(modeName == __SDN_LAB_MCS_18){
                    mcs = 18;
                }else if(modeName == __SDN_LAB_MCS_19){
                    mcs = 19;
                }else if(modeName == __SDN_LAB_MCS_110){
                    mcs = 110;
                }else if(modeName == __SDN_LAB_MCS_20){
                    mcs = 20;
                }else if(modeName == __SDN_LAB_MCS_21){
                    mcs = 21;
                }else if(modeName == __SDN_LAB_MCS_22){
                    mcs = 22;
                }else if(modeName == __SDN_LAB_MCS_23){
                    mcs = 23;
                }else if(modeName == __SDN_LAB_MCS_24){
                    mcs = 24;
                }else if(modeName == __SDN_LAB_MCS_25){
                    mcs = 25;
                }else if(modeName == __SDN_LAB_MCS_26){
                    mcs = 26;
                }else if(modeName == __SDN_LAB_MCS_27){
                    mcs = 27;
                }else if(modeName == __SDN_LAB_MCS_28){
                    mcs = 28;
                }else if(modeName == __SDN_LAB_MCS_40){
                    mcs = 40;
                }else if(modeName == __SDN_LAB_MCS_41){
                    mcs = 41;
                }else if(modeName == __SDN_LAB_MCS_42){
                    mcs = 42;
                }else if(modeName == __SDN_LAB_MCS_43){
                    mcs = 43;
                }else if(modeName == __SDN_LAB_MCS_44){
                    mcs = 44;
                }else if(modeName == __SDN_LAB_MCS_45){
                    mcs = 45;
                }else if(modeName == __SDN_LAB_MCS_46){
                    mcs = 46;
                }else if(modeName == __SDN_LAB_MCS_47){
                    mcs = 47;
                }else if(modeName == __SDN_LAB_MCS_48){
                    mcs = 48;
                }else if(modeName == __SDN_LAB_MCS_49){
                    mcs = 49;
                }
                return mcs;
            };

            /**
             * summary
             */ 
            void Summary(const char * filename){
                if (filename){
                    std::cout << "---------------------" << std::endl;
                    std::cout << "| " << filename << std::endl;
                    std::cout << "---------------------" << std::endl;
                }
                Summary();
            }
            void Summary(){
                std::cout << "PacketContext (" << this->id <<")" << std::endl;
                if(this->isEmpty){
                    std::cout << " - Empty: true" << std::endl;
                }else{
                    std::cout << " - MAC Layer" << std::endl;
                    std::cout << "   - PacketSize:     " << this->macPacketSize << std::endl;
                    std::cout << "   - SourMacAddr:    " << this->sourMacAddr << std::endl;
                    std::cout << "   - DestMacAddr:    " << this->destMacAddr << std::endl;
                    std::cout << "   - TxMacAddr:      " << this->txMacAddr << std::endl;
                    std::cout << "   - RxMacAddr:      " << this->rxMacAddr << std::endl;
                    std::cout << "   - BSSID:          " << this->bssid << std::endl;
                    std::cout << " - Physical Layer" << std::endl;
                    std::cout << "   - PacketSize:     " << this->phyPacketSize << std::endl;
                    std::cout << "   - StartTime:      " << this->startTime << std::endl;
                    std::cout << "   - EndTime:        " << this->endTime << std::endl;
                    std::cout << "   - Bandwidth:      " << this->bandwidth << std::endl;
                    std::cout << "   - Mcs_in:         " << this->mcs_in << std::endl;
                    std::cout << "   - per:            " << this->per << std::endl;
                    std::cout << "   - snr:            " << this->snr << std::endl;
                    std::cout << "   - RxPower:        " << this->rxPower << std::endl;
                    std::cout << "   - InterferePower: " << this->interferePower << std::endl;
                    std::cout << "   - NodeIndex:      " << this->nodeIndex << std::endl;
                }
            }

            /*** whether has any overhead ***/
            // snn
            bool HasOverhead(){
                return this->overhead != NULL;
            }
            
            /*** Get & Set ***/
            // empty
            bool IsEmpty() const{
                return this->isEmpty;
            }
            // Tx, Rx, Source, Destination Mac address and BSSID
            void SetAllMacAddr(const ns3::WifiMacHeader *hdr){
                if(hdr){
                    // set Tx Mac address & Rx Mac address
                    this->txMacAddr = hdr->GetAddr2();
                    this->rxMacAddr = hdr->GetAddr1();
                    // set dest, source Mac address and BSSID
                    // toDS = 0, fromDS = 0 (STA to STA, inside a AP)
                    if (!hdr->IsToDs() && !hdr->IsFromDs()){
                        this->sourMacAddr = this->txMacAddr;
                        this->destMacAddr = this->rxMacAddr;
                        this->bssid = hdr->GetAddr3();
                    }
                    // toDS = 0, fromDS = 1 (AP to STA, maybe through APs)
                    if (!hdr->IsToDs() && hdr->IsFromDs()){
                        this->sourMacAddr = hdr->GetAddr3();
                        this->destMacAddr = this->rxMacAddr;
                        this->bssid = this->txMacAddr;
                    }
                    // toDS = 1, fromDS = 0 (STA to AP, maybe through APs)
                    if (hdr->IsToDs() && !hdr->IsFromDs()){
                        this->sourMacAddr = this->txMacAddr;
                        this->destMacAddr = hdr->GetAddr3();
                        this->bssid = this->rxMacAddr;
                    }
                    // toDS = 1, fromDS = 1 (AP to AP)
                    // from a distribution system to another distribution system
                    if (hdr->IsToDs() && hdr->IsFromDs()){
                        this->sourMacAddr = hdr->GetAddr4();
                        this->destMacAddr = hdr->GetAddr3();
                        // bssid has no value when crossing two APs
                        // manually set to broadcast
                        this->bssid = __SDN_LAB_MAC_BROADCAST_ADDR;
                    }
                }
            }
            ns3::Mac48Address GetSourMacAddr() const{
                return this->sourMacAddr;
            }
            ns3::Mac48Address GetDestMacAddr() const{
                return this->destMacAddr;
            }
            ns3::Mac48Address GetTxMacAddr() const{
                return this->txMacAddr;
            }
            ns3::Mac48Address GetRxMacAddr() const{
                return this->rxMacAddr;
            }
            ns3::Mac48Address GetBSSID() const{
                return this->bssid;
            }
            // Mac packet size  
            void SetMacPacketSize(uint32_t packetSize){
                this->macPacketSize = packetSize;
            }
            uint32_t GetMacPacketSize() const{
                return this->macPacketSize;
            }
            // Phy packet size
            void SetPhyPacketSize(uint32_t packetSize){
                this->phyPacketSize = packetSize;
                this->isEmpty = false;
            }
            uint32_t GetPhyPacketSize() const{
                return this->phyPacketSize;
            }
            // startTime
            void SetStartTime(double time){
                this->startTime = time;
                this->isEmpty = false;
            }
            double GetStartTime() const{
                return this->startTime;
            }
            // end time
            void SetEndTime(double time){
                this->endTime = time;
                this->isEmpty = false;
            }
            double GetEndTime() const{
                return this->endTime;
            }
            // per
            void SetPer(double per){
                this->per = per;
                this->isEmpty = false;
            }
            double GetPer() const{
                return this->per;
            }
            // snr
            void SetSnr(double snr){
                this->snr = snr;
                this->isEmpty = false;
            }
            double GetSnr() const{
                return this->snr;
            }
            // Rx power in Watt
            void SetRxPower(double power){
                this->rxPower = power;
                this->isEmpty = false;
            }
            double GetRxPower() const{
                return this->rxPower;
            }
            // interefernce power in Watt
            double GetInterferePower() const{
                return this->interferePower;
            }
            // coming mcs & bandwidth
            void SetMcsInAndBandwidth(std::string modeName){
                // calculate bandwidth & mcs_in from the mode name
                uint32_t bandwidth = _PacketContext::ModeName2Bandwidth(modeName);
                unsigned int mcs_in = _PacketContext::ModeName2MCS(modeName);
                // assign
                this->bandwidth = bandwidth;
                this->mcs_in = mcs_in;
                this->isEmpty = false;
            }
            // MCS - Coming
            unsigned int GetMCSIn() const{
                return this->mcs_in;
            }
            // bandwidth
            uint32_t GetBandwidth() const{
                return this->bandwidth;
            }
            // MCS - Predicted
            void SetMCSPredict(unsigned int mcs_predict){
                this->mcs_predict = mcs_predict;
            }
            unsigned int GetMCSPredict() const{
                return this->mcs_predict;
            }
            // is received
            void SetReceived(){
                this->isReceived = true;
            }
            void SetNotReceived(){
                this->isReceived = false;
            }
            bool IsReceived() const{
                return this->isReceived;
            }
            // node
            void SetNodeIndex(uint32_t nodeIndex){
                this->nodeIndex = nodeIndex;
            }
            uint32_t GetNodeIndex() const{
                return this->nodeIndex;
            }
            // overhead
            void SetOverhead(Overhead * overhead){
                if(overhead){
                    this->overhead = overhead;
                }
                this->isEmpty = false;
            }
            Overhead * GetOverhead() const{
                return this->overhead;
            }

            /*** Operators Overload ***/
            // copy constructor
            _PacketContext(const _PacketContext & context){
                this->isEmpty           = context.isEmpty;
                this->macPacketSize     = context.macPacketSize;
                this->sourMacAddr       = context.sourMacAddr;
                this->destMacAddr       = context.destMacAddr;
                this->txMacAddr         = context.txMacAddr;
                this->rxMacAddr         = context.rxMacAddr;
                this->bssid             = context.bssid;
                this->phyPacketSize     = context.phyPacketSize;
                this->startTime         = context.startTime;
                this->endTime           = context.endTime;
                this->bandwidth         = context.bandwidth;
                this->mcs_in            = context.mcs_in;
                this->mcs_predict       = context.mcs_predict;
                this->per               = context.per;
                this->snr               = context.snr;
                this->rxPower           = context.rxPower;
                this->interferePower    = context.interferePower;
                this->isReceived        = context.isReceived;
                this->nodeIndex         = context.nodeIndex;
                // overhead - SNN
                this->overhead          = context.overhead ? context.overhead->Copy() : NULL;
            };
            // assign
            _PacketContext& operator=(const _PacketContext &context){
                this->isEmpty           = context.isEmpty;
                this->macPacketSize     = context.macPacketSize;
                this->sourMacAddr       = context.sourMacAddr;
                this->destMacAddr       = context.destMacAddr;
                this->txMacAddr         = context.txMacAddr;
                this->rxMacAddr         = context.rxMacAddr;
                this->bssid             = context.bssid;
                this->phyPacketSize     = context.phyPacketSize;
                this->startTime         = context.startTime;
                this->endTime           = context.endTime;
                this->bandwidth         = context.bandwidth;
                this->mcs_in            = context.mcs_in;
                this->mcs_predict       = context.mcs_predict;
                this->per               = context.per;
                this->snr               = context.snr;
                this->rxPower           = context.rxPower;
                this->interferePower    = context.interferePower;
                this->isReceived        = context.isReceived;
                this->nodeIndex         = context.nodeIndex;
                // overhead - SNN
                this->overhead          = context.overhead ? context.overhead->Copy() : NULL;
                return *this;
            };
        };
        
        /*** redefined other relevant type names ***/
        typedef _PacketContext PacketContext;
        typedef _PacketContext ContextFactory;
    }
#endif