#pragma once
#ifndef __SDN_LAB_STATIONLIST_H
    #define __SDN_LAB_STATIONLIST_H
    #include <iostream>
    #include "Modules/Toolbox/Error.h"  // Error to throw
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "PacketContext.h"
    #include "Station.h"
    #include "Mac.h"                    // Mac constants
    // Memory Cost (base) 24
    namespace SdnLab{
        class _StationList{
            private:
            unsigned int staMemSize = 0;                // the memory allocated to each station
            // track station list
            PtrStation * staList    = NULL;             // station list (at the beginning node)
            unsigned int staListLen = 0;                // the station list length (default 0)
            unsigned int staListMaxLen = 0;             // the station list maximal length

            // disexpose constructor
            // constructor
            _StationList(){};
            _StationList(unsigned int memorySize, unsigned int stationMaxNum){
                // remove the memory cost for _StationList
                memorySize = memorySize - sizeof(_StationList);
                // set the memory for each station
                this->staListMaxLen = stationMaxNum;
                if (this->staListMaxLen > 0){
                    try{
                        this->staList = new PtrStation[this->staListMaxLen];
                        for(unsigned int i = 0; i < this->staListMaxLen; ++i){
                            this->staList[i] = NULL;
                        }
                        this->staMemSize = (unsigned int)(memorySize/this->staListMaxLen);
                    }catch(const std::bad_alloc & e){
                        Toolbox::Error err("/Components", "StationList.h", "_StationList", "_StationList", "Cannot support too many stations");
                        err.SetType2MemoryShortage();
                        throw err;
                    }
                }
            };
            // deconstructor
            ~_StationList(){
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
            static _StationList * Create(unsigned int memorySize, unsigned int stationMaxNum){
                _StationList * stationList = new _StationList(memorySize, stationMaxNum);
                return stationList;
            };
            /*
             * Destroy
             */
            static void Destory(_StationList * ptrStationList){
                if(ptrStationList){
                    delete ptrStationList;
                }
            }
            
            /**
             * Summary the configuration
             */
            static void Summary(){
                std::cout << "SdnLab::_StationList      " << std::endl;
                std::cout << " - Memory(base):         " << sizeof(_StationList) << std::endl;
                std::cout << std::endl;
                Station::Summary();
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
             * add a station or add the context to an existing station
             * @context:  the packetContext
             */
            bool AddStationOrContext(PacketContext context){
                // init varables
                bool isAddSta = false;
                bool isAddContext = false;
                unsigned int i = 0;
                ns3::Mac48Address sourMacAddr;

                // check whether the station exists or not
                if(context){
                    sourMacAddr = context->GetSourMacAddr();
                    // check when the source Mac address is valid
                    if (sourMacAddr != __SDN_LAB_MAC_BROADCAST_ADDR){
                        for(; i < this->staListLen; ++i){
                            // jump out - should add because empty means previous ones have no such address
                            if(!this->staList[i]){
                                isAddSta = true;
                                break;
                            }
                            // jump out 
                            // - should ad
                            if (this->staList[i]->GetMacAddress() == sourMacAddr){
                                isAddContext = true;
                                break;
                            }
                        }
                    }
                    // add STA if should
                    if(isAddSta){
                        this->staList[i] = new Station(sourMacAddr, this->staMemSize);
                    }
                    // add context if should
                    if(isAddContext){
                        this->staList[i]->AddData(context->GetStartTime(), context->GetSnr(), context->GetRxPower());
                    }
                }
                return isAddSta && isAddContext;
            };
        };

        /*** redefined other relevant type names ***/
        typedef _StationList StationListFactory;
        typedef _StationList * StationList;
    }
#endif