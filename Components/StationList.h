#pragma once
#ifndef __SDN_LAB_STATIONLIST_H
    #define __SDN_LAB_STATIONLIST_H
    #include <iostream>
    #include "Modules/Toolbox/Error.h"          // Error to throw
    #include <fstream>
    #include "ns3/mac48-address.h"              // support Mac48Address
    #include "PacketContext.h"
    #include "NNData.h"
    #include "Station.h"
    #include "Mac.h"                            // Mac constants
    #define __SDN_LAB_STATIONLIST_MEMORY_COST_BASE 24
    /**
     * calculate the required memory cost
     * @staNum:         the maximal number of stations
     * @staDataNum:     the maximal number of data for each station
     */
    #define __SDN_LAB_STATIONLIST_MEMORY_COST_REQ(staNum, staDataNum) __SDN_LAB_STATIONLIST_MEMORY_COST_BASE+staNum*(__SDN_LAB_STATION_MEMORY_COST_BASE + staDataNum*__SDN_LAB_STATION_MEMORY_COST_DATA)
    // Memory Cost (base) 24
    namespace SdnLab{
        class _StationList{
            private:
            unsigned int staMemSize = 0;                // the memory allocated to each station
            // track station list
            PtrStation * staList    = NULL;             // station list (at the beginning node)
            unsigned int staListLen = 0;                // the station list length (default 0)
            unsigned int staListMaxLen = 0;             // the station list maximal length

            // NN Data shared accross Python and C/C++
            #if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
                SdnLab::NNData nnData;
            #endif

            // disexpose constructor
            // constructor
            _StationList(){};
            _StationList(unsigned int memorySize, unsigned int stationMaxNum){
                // remove the memory cost for _StationList
                if(memorySize <= sizeof(_StationList)){
                    memorySize = 0;
                }else{
                    memorySize = memorySize - sizeof(_StationList);
                }
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
            
            // debug 
            #ifdef __SDN_LAB_DEBUG
                /**
                 * Summary the configuration
                 */
                static void Summary(void){
                    std::cout << "SdnLab::_StationList      " << std::endl;
                    std::cout << " - Memory(base):         " << sizeof(_StationList) << std::endl;
                    std::cout << std::endl;
                    StationFactory::Summary();
                };
                static void Summary(std::string & filepath){
                    std::fstream file;
                    file.open(filepath, std::fstream::in | std::fstream::app);
                    file << "SdnLab::_StationList      " << '\n';
                    file << " - Memory(base):         " << std::to_string(sizeof(_StationList)) << '\n';
                    file << '\n';
                    file.close();
                    StationFactory::Summary(filepath);
                };
                void Summary2File(std::string & filepath, unsigned int datalen = 0){
                    std::cout<<"StalistLen = " << this->staListLen << '\n';
                    unsigned int i;
                    for(i = 0; i < this->staListLen; i++){
                        this->staList[i]->Summary2File(filepath, datalen);
                    }
                }
                unsigned int GetStaMemSize(){
                    return this->staMemSize;
                }
            #endif

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
                unsigned int i;
                ns3::Mac48Address sourMacAddr;

                // check whether the station exists or not
                if(!context.IsEmpty()){
                    sourMacAddr = context.GetSourMacAddr();
                    // check when the source Mac address is valid
                    if (sourMacAddr != __SDN_LAB_MAC_BROADCAST_ADDR){
                        // view the entire list !
                        for(i = 0; i < this->staListMaxLen; ++i){
                            // jump out - empty space
                            // <NOTE>
                            // previous stations don't have the same Mac address
                            if(!this->staList[i]){
                                isAddSta = true;
                                isAddContext = true;
                                break;
                            }
                            // jump out - same Mac address
                            if (*this->staList[i] == sourMacAddr){
                                isAddContext = true;
                                break;
                            }
                        }
                    }
                    // add STA if should
                    if(isAddSta){
                        this->staList[i] = new Station(sourMacAddr, this->staMemSize);
                        ++this->staListLen;
                    }
                    // add context if should
                    if(isAddContext){
                        this->staList[i]->AddData(context.GetEndTime(), context.GetSnr(), context.GetRxPower());
                    }
                }
                return isAddSta || isAddContext;
            };

            /**
             * predict
             */
            #if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
                void Predict(){
                    unsigned int i;
                    for(i = 0; i < this->staListLen; ++i){
                        this->nnData.SetFeatures(this->staList[i]);
                        // SNN-Vincent
                        // SNN
                        #if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN)
                            this->nnData.GetPredict(this->staList[i]);
                        #endif
                        // SNN+
                        // Minstrel-AI-Dist
                        #if defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
                            this->nnData.GetPredicts(this->staList[i]);
                        #endif
                    }
                };
            #endif
        };

        /*** redefined other relevant type names ***/
        typedef _StationList StationListFactory;
        typedef _StationList * StationList;
    }
#endif