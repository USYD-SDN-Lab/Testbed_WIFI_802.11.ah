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
        PtrStation * staList    = NULL;             // station list (at the beginning node)
        unsigned int staListLen = 0;                // the station list length (default 0)
        unsigned int staListMaxLen;                 // the station list maximal length

        
        public:
        /**
         * Init
         * <INPUT>
         * @memorySize:         the memory can be allocated to the station list (bytes)
         * @stationMaxNum:      the maximal number of stations
         * <return>
         * 
         */
        unsigned int Init(unsigned int memorySize, unsigned int stationMaxNum){
            // remove the memory cost for StationList
            memorySize = memorySize - sizeof(StationList);
            // set the memory for each station
            this->staListLen = stationMaxNum;
            if (this->staListLen > 0){
                this->staList = new PtrStation[this->staListLen];
                this->staMemSize = (unsigned int)(memorySize/this->staListLen);
            }
            return this->staMemSize;
        }
        /* Deconstructor */
        ~StationList(){
            Clear();
        }

        /**
         * Clear all allocated space
         */ 
        void Clear(){
            // release memory
            if(this->staList){
                for(unsigned int i = 0; i < this->staListMaxLen; i++){
                    if(this->staList[i]){
                        delete this->staList[i];
                    }
                }
                delete[] this->staList;
            }
            // reset pointers to NULL
            this->staList = NULL;
            // counter set to 0
            this->staListLen = 0;
        };

        /**
         * add a station
         * \param packetContext:  the packetContext
         */
        bool AddStation(PtrPacketContext packetContext){
            
        }
    };
#endif