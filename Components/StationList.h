#pragma once
#ifndef __SDN_LAB_STATIONLIST_H
    #define __SDN_LAB_STATIONLIST_H
    #include <iostream>
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "PacketContext.h"
    #include "Station.h"
    #include "StationData.h"
    namespace SdnLab{
        class StationList{
            private:
            unsigned int staMemSize = 0;                // the memory allocated to each station
            // track station list
            PtrStation * staList    = NULL;             // station list (at the beginning node)
            unsigned int staListLen = 0;                // the station list length (default 0)
            unsigned int staListMaxLen = 0;             // the station list maximal length

            // disexpose constru
            // constructor
            StationList(){};
            StationList(unsigned int memorySize, unsigned int stationMaxNum){
                // remove the memory cost for StationList
                memorySize = memorySize - sizeof(StationList);
                // set the memory for each station
                this->staListMaxLen = stationMaxNum;
                if (this->staListMaxLen > 0){
                    this->staList = new PtrStation[this->staListMaxLen];
                    this->staMemSize = (unsigned int)(memorySize/this->staListMaxLen);
                }
            };
            // deconstructor
            ~StationList(){
                Clear();
            };
            
            public:
            /**
             * Create
             * <INPUT>
             * @memorySize:         the memory can be allocated to the station list (bytes)
             * @stationMaxNum:      the maximal number of stations
             * <return>
             * 
             */
            static StationList * Create(unsigned int memorySize, unsigned int stationMaxNum){
                StationList * stationList = new StationList(memorySize, stationMaxNum);
                return stationList;
            };
            /*
             * Destroy
             */
            static void Destory(StationList * ptrStationList){
                if(ptrStationList){
                    delete ptrStationList;
                }
            }
            
            /**
             * Summary the configuration
             */
            void Summary(){
                std::cout << "SdnLab::StationList      " << std::endl;
                std::cout << " - Memory(base):         " << sizeof(StationList) << std::endl;
                std::cout << " - Station List:         " << this->staListLen << "/" << this->staListMaxLen << " (used/total)" << std::endl;
                std::cout << " - SdnLab::Station       " << std::endl;
                std::cout << "   - Memory(base):       " << sizeof(Station) << std::endl;
                std::cout << "   - Memory(avialble):   " << this->staMemSize << std::endl;
                std::cout << "   - SdnLab::StationData:" << std::endl;
                std::cout << "     - Memory(base):     " << sizeof(StationData) << std::endl;
            };

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
             * @packetContext:  the packetContext
             */
            bool AddStation(PtrPacketContext packetContext){
                bool shouldAdd = false;

                // check whether the station exists or not
                if(packetContext){
                    
                }
                return false;
            };
        };

        /*** redefined other relevant type names ***/
        typedef StationList * PtrStationList;
    }
#endif