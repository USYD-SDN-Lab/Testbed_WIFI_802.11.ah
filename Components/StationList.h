#pragma once
#ifndef __SDN_LAB_STATIONLIST_H
    #define __SDN_LAB_STATIONLIST_H
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "PacketContext.h"
    #include "Station.h"
    
    class StationList{
        private:
        unsigned int staMemSize = 0;                // the memory allocated to each station
        // track station list
        unsigned int staListLen = 0;                // the station list length (default 0)
        unsigned int staListMaxLen;                 // the station list maximal length
        PtrStation * staList    = NULL;             // station list (at the beginning node)
        
        public:
        /**
         * constructor
         * @memorySize:         the memory can be allocated to the station list (bytes)
         * @stationMaxNum:      the maximal number of stations
         */
        StationList(unsigned int memorySize, unsigned int stationMaxNum){
            this->staListLen = stationMaxNum;
            if (this->staListLen > 0){
                this->staList = new PtrStation[this->staListLen];
                this->staMemSize = (int)(memorySize/this->staListLen)
            }
        }
        /* deconstructor */
        ~StationList(){
            ClearStationList();
        }

        /**
         * clean the Station Data List
         */ 
        void ClearStationList(){
            // clean the 
            for(unsigned int i = 0; i < this->staListMaxLen; i++){
                if(this->staList[i]){
                    delete this->staList[i];
                }
            }
            delete[] this->staList;
        };

        /**
         * add a station
         * @packetContext:  the packetContext
         */
        bool AddStation(PtrPacketContext packetContext){
            
        }
    };
#endif