#pragma once
#ifndef __SDN_LAB_STATION_H
    #define __SDN_LAB_STATION_H
    #include <iostream>
    #include <climits>                  // support `UINT_MAX`
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "Modules/Toolbox/Error.h"  // Error to throw
    #include "NNData.h"
    #define __SDN_LAB_STATION_MEMORY_COST_BASE              384     // Memory Cost (base): 384
    #define __SDN_LAB_STATION_MEMORY_COST_DATA              32      // Memory Cost (data): 32
    // _DATA item types
    #define __SDN_LAB_STATION_DATA_ITEM_TYPE_TIME           1
    #define __SDN_LAB_STATION_DATA_ITEM_TYPE_SNR            2
    #define __SDN_LAB_STATION_DATA_ITEM_TYPE_RXPOWER        3
    #define __SDN_LAB_STATION_DATA_ITEM_TYPE_BANDWIDTH      4
    namespace SdnLab{
        class Station{
            private:
            /*** private data structures ***/
            // _DATA: 8*4 = 32 bytes 
            // struct takes the highest item-wise memory cost as the item-wise memory cost
            // <WARNING>
            // althought `unsigned int` takes 4 bytes, `double` increases the memory cost to 8 bytes
            struct _Data{
                double time;                        // the time point of this data
                double snr;                         // SNR
                double rxPower;                     // rxPower
                unsigned int bandwidth;             // bandwidth (Hz)
            };
            /*** members ***/
            // Minstrel-SNN
            unsigned int nnMcsPredict[__SDN_LAB_MCS_NUM];   // MCS
            double nnMcsActivateTime[__SDN_LAB_MCS_NUM];    // MCS activate time
            // common
            ns3::Mac48Address macAddr;              // the station mcs address
            unsigned int datalistLen = 0;           // the station data length
            unsigned int datalistMaxLen = 0;        // the station data maximal length
            _Data * datalist = NULL;                // station list
            unsigned int ptrb_datalist = 0;         // pointer -> station data list beginning
            unsigned int ptre_datalist = 0;         // pointer -> station data list end
            // Minstrel-RSNN
            double lastBeaconTime = 0;              // the last beacon time
            double lastBeaconSNR = 0;               // the last beacon SNR
            double lastBeaconRxPower = 0;           // the last beacond RxPower
            double lastBeaconTotalWeight = 0;
            /*** inner functions ***/
            /**
             * get the data list of a certain type (time, snr, rxPower or bandwidth)
             * <INPUT>
             * @list:           the pointer to the list (void *, so we can't move pointer before changing it into a certain type with a known memory cost)
             * @listDataType:   the type of data (please check macros like __SDN_LAB_STATION_DATA_ITEM_TYPE_XXX in the macro definition)
             * @listMaxLen:     the list length to arrange data
             * <OUTPUT>
             * @false:          list is NULL or listMaxLen is 0
             * @true:           otherwise
             * <ERROR>
             * @err1:           `listDataType` set to an unkown type
             * @err2:           `listMaxLen` set to `UINT_MAX`: causes infinite loops
             */
            bool GetDataList(void * list, unsigned int listDataType, unsigned int listMaxLen){
                // do nothing when not arrange the memory or 0 available memory
                if(!list || listMaxLen == 0){
                    return false;
                }
                // possible errors (should be avoided in programming)
                Toolbox::Error err1("/Components/", "Station.h", "Station", "GetDataList()", "`listDataType` set to an unkown type");
                err1.SetType2IllegalParameters();
                if (listDataType != __SDN_LAB_STATION_DATA_ITEM_TYPE_TIME && listDataType != __SDN_LAB_STATION_DATA_ITEM_TYPE_SNR && listDataType != __SDN_LAB_STATION_DATA_ITEM_TYPE_RXPOWER && listDataType != __SDN_LAB_STATION_DATA_ITEM_TYPE_BANDWIDTH){
                    throw err1;
                }
                Toolbox::Error err2("/Components/", "Station.h", "Station", "GetDataList()", "`listMaxLen` set to `UINT_MAX`: causes infinite loops");
                err2.SetType2IllegalParameters();
                if (listMaxLen == UINT_MAX){
                    throw err2;
                }
                // append data into the list
                unsigned int i = this->ptre_datalist;
                unsigned int j = listMaxLen - 1;
                // if given memory is over `datalist`, we should assign 0 at the end
                if(listMaxLen > this->datalistLen){
                    // assign data (won't run if `datalistLen == 0`)
                    // <NOTE>
                    // `j<0` means j overflows to `UINT_MAX`
                    for(; j >= this->datalistLen & j < listMaxLen; --j){
                        switch(listDataType){
                            case __SDN_LAB_STATION_DATA_ITEM_TYPE_TIME:
                            case __SDN_LAB_STATION_DATA_ITEM_TYPE_SNR:
                            case __SDN_LAB_STATION_DATA_ITEM_TYPE_RXPOWER:
                                *((double *)list + j) = 0;
                                break;
                            case __SDN_LAB_STATION_DATA_ITEM_TYPE_BANDWIDTH:
                                *((unsigned int *)list + j) = 0;
                                break;
                        }
                    }
                }
                // -----------------        listMaxLen (possible)
                // ============             datalistLen
                // ------                   listMaxLen (possible)
                // if given memory is larger, `j` is set to `datalistLen - 1` now (always true when `datalistLen == 0`)
                // if given memory is smaller or equal, `j` is set to `listMaxLen - 1` now
                //
                // assign data (won't run if `datalistLen == 0`)
                // <NOTE>
                // `j<0` means j overflows to `UINT_MAX`
                for(; j >= 0 & j < listMaxLen; --j){
                    // assign
                    switch(listDataType){
                        case __SDN_LAB_STATION_DATA_ITEM_TYPE_TIME:
                            *((double *)list + j) = this->datalist[i].time;
                            break;
                        case __SDN_LAB_STATION_DATA_ITEM_TYPE_SNR:
                            *((double *)list + j) = this->datalist[i].snr;
                            break;
                        case __SDN_LAB_STATION_DATA_ITEM_TYPE_RXPOWER:
                            *((double *)list + j) = this->datalist[i].rxPower;
                            break;
                        case __SDN_LAB_STATION_DATA_ITEM_TYPE_BANDWIDTH:
                            *((unsigned int *)list + j) = this->datalist[i].bandwidth;
                            break;
                    }
                    // shift `i`
                    // when ptrb_datalist <= ptre_datalist, `--i`: this->ptre_datalist -> this->ptrb_datalist
                    // when ptrb_datalist >  ptre_datalist, `--i`: this->ptre_datalist -> 0 -> this->datalistLen -1 -> this->ptrb_datalist
                    if (i > 0){
                        --i;
                    }else{
                        i = this->datalistLen -1;   // only happens when ptrb_datalist >  ptre_datalist
                    }
                }
            }

            public:
            /**
             * constructor
             * @macAddr:    the Mac Address
             * @memorySize: the memory can be allocated to each station (bytes)
             */
            Station(ns3::Mac48Address macAddr, unsigned int memorySize){
                // set the mac address
                this->macAddr = macAddr;
                // set the datalist & its control tags
                if(memorySize < sizeof(Station) + sizeof(_Data)){
                    this->datalistMaxLen = 0;
                }else{
                    memorySize = memorySize - sizeof(Station);
                    this->datalistMaxLen = (unsigned int)(memorySize / sizeof(_Data));
                    try{
                        this->datalist = new _Data[this->datalistMaxLen];
                    }catch(const std::bad_alloc & e){
                        Toolbox::Error err("/Components", "Station.h", "Station", "Station", "Cannot support that much data");
                        err.SetType2MemoryShortage();
                        throw err;
                    }
                }
                // NN
                unsigned int i = 0;
                for(; i < __SDN_LAB_MCS_NUM; ++i){
                    this->nnMcsPredict[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                    this->nnMcsActivateTime[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                }
            };
            /**
             * deconstructor
             */
            ~Station(){
                if (this->datalist){
                    delete[] this->datalist;
                }
            };

            /**
             * add 1 Station Data into the list
             * <INPUT>
             * @time:       the time point (second)
             * @snr:        SINR (dB)
             * @rxPower:    rx power (Watt)
             * @bandwidth:  bandwidth (Hz)
             * <OUTPUT>
             * bool, whether success or failure
             */
            void AddData(double time, double snr, double rxPower, unsigned int bandwidth){
                #ifdef __SDN_LAB_RA_MINSTREL_SNN_PLUS
                    // only update when we have received a beacon
                    if(this->lastBeaconTime > 0){
                        //std::cout<<"time="<<time<<std::endl;
                        //std::cout<<"lastBeaconTime"<<this->lastBeaconTime<<std::endl;
                        //std::cout<<"rxPower"<<rxPower<<std::endl;
                        //NS_ASSERT(false);
                        // update the average SNR
                        double addedWeight = (time - this->lastBeaconTime);
                        this->lastBeaconSNR += addedWeight*snr;
                        this->lastBeaconRxPower += addedWeight*rxPower;
                        this->lastBeaconTotalWeight += addedWeight;
                    }
                #else
                    // move the end pointer (if the datalist is not empty)
                    if(this->datalistLen > 0){
                        ++(this->ptre_datalist);
                        // if move over the limit, set to 0
                        if (this->ptre_datalist == this->datalistMaxLen){
                            this->ptre_datalist = 0;
                        }
                    }
                    // add data
                    this->datalist[this->ptre_datalist].time = time;
                    this->datalist[this->ptre_datalist].snr = snr;
                    this->datalist[this->ptre_datalist].rxPower = rxPower;
                    this->datalist[this->ptre_datalist].bandwidth = bandwidth;
                    ++(this->datalistLen);
                    // move the beginning pointer (if the datalist length is over the limit)
                    if (this->datalistLen > this->datalistMaxLen){
                        ++(this->ptrb_datalist);
                        // if move over the limit, set to 0
                        if (this->ptrb_datalist == this->datalistMaxLen){
                            this->ptrb_datalist = 0;
                        }
                        // set the datalist length to the limit
                        this->datalistLen = this->datalistMaxLen;
                    }
                #endif
            };

            /**
             * Beacon Data (plz call these functions before `UpdateBeaconTime`) 
             */
            double getBDLastBeaconTime(){
                return this->lastBeaconTime;                
            };
            double getBDLastBeaconSNR(){
                if(this->lastBeaconTotalWeight == 0){
                    this->lastBeaconTotalWeight = 1;
                }
                return this->lastBeaconSNR/this->lastBeaconTotalWeight;
            };
            double getBDLastBeaconRxPower(){
                if(this->lastBeaconTotalWeight == 0){
                    this->lastBeaconTotalWeight = 1;
                }
                return this->lastBeaconRxPower/this->lastBeaconTotalWeight;
            };

            /**
             * update the beacon time
             */
            void UpdateBeaconTime(double time){
                #ifdef __SDN_LAB_RA_MINSTREL_SNN_PLUS
                    // add the last beacon data into the datalist
                    if(this->lastBeaconTime > 0){
                        // std::cout<< "time = " << time << std::endl;
                        // std::cout<< "lastBeaconTime = " << this->lastBeaconTime << std::endl;
                        // std::cout<< "total SNR = " << this->lastBeaconSNR << std::endl;
                        // std::cout<< "total rxPower = " << this->lastBeaconRxPower << std::endl;
                        // std::cout<< "total weight = " << this->lastBeaconTotalWeight << std::endl;
                        // std::cout<< "average SNR = " << this->lastBeaconSNR/this->lastBeaconTotalWeight << std::endl;
                        // std::cout<< "average rxPower = " << this->lastBeaconRxPower/this->lastBeaconTotalWeight << std::endl;
                        // if(this->lastBeaconTime >= 4){
                        //     NS_ASSERT(false);
                        // }
                        
                        // move the end pointer (if the datalist is not empty)
                        if(this->datalistLen > 0){
                            ++(this->ptre_datalist);
                            // if move over the limit, set to 0
                            if (this->ptre_datalist == this->datalistMaxLen){
                                this->ptre_datalist = 0;
                            }
                        }
                        // add data
                        if(this->lastBeaconTotalWeight == 0){
                            this->lastBeaconTotalWeight = 1;
                        }
                        this->datalist[this->ptre_datalist].time = this->lastBeaconTime;
                        this->datalist[this->ptre_datalist].snr = this->lastBeaconSNR/this->lastBeaconTotalWeight;
                        this->datalist[this->ptre_datalist].rxPower = this->lastBeaconRxPower/this->lastBeaconTotalWeight;
                        this->datalist[this->ptre_datalist].bandwidth = 0;
                        ++(this->datalistLen);
                        // move the beginning pointer (if the datalist length is over the limit)
                        if (this->datalistLen > this->datalistMaxLen){
                            ++(this->ptrb_datalist);
                            // if move over the limit, set to 0
                            if (this->ptrb_datalist == this->datalistMaxLen){
                                this->ptrb_datalist = 0;
                            }
                            // set the datalist length to the limit
                            this->datalistLen = this->datalistMaxLen;
                        }
                    }
                #endif

                this->lastBeaconTime = time;
                this->lastBeaconSNR = 0;
                this->lastBeaconRxPower = 0;
                this->lastBeaconTotalWeight = 0;
            }

            #ifdef __SDN_LAB_DEBUG
                /**
                 * Summary the configuration
                 */
                static void Summary(void){
                    std::cout << "SdnLab::Station " << std::endl;
                    std::cout << " - Memory(base):" << sizeof(Station) << std::endl;
                    std::cout << " - Memory(data):" << sizeof(_Data) << std::endl;
                };
                static void Summary(std::string & filepath){
                    std::fstream file;
                    file.open(filepath, std::fstream::in | std::fstream::app);
                    file << "SdnLab::Station " << '\n';
                    file << " - Memory(base):" << sizeof(Station) << '\n';
                    file << " - Memory(data):" << sizeof(_Data) << '\n';
                    file.close();
                };
                void Summary2File(std::string & filepath, unsigned int datalen = 0, bool isNNData = false){
                    std::fstream file;
                    unsigned int i;
                    // open the file
                    file.open(filepath, std::fstream::in | std::fstream::app);
                    if(!isNNData){
                        // retrieve the time list & rxPower list
                        if (datalen == 0){
                            datalen = this->datalistMaxLen;
                        }
                        double * timeList = new double[datalen];
                        double * rxPowerList = new double[datalen];
                        unsigned int * bandwidthList = new unsigned int[datalen];
                        GetTimeList(timeList, datalen);
                        GetRxPowerList(rxPowerList, datalen);
                        GetBandwidthList(bandwidthList, datalen);

                        // print all time
                        file << this->macAddr << ',';
                        for(i = 0; i < datalen; ++i){
                            file << timeList[i];
                            if (i < datalen - 1){
                                file << ',';
                            }else{
                                file << '\n';
                            }
                        }
                        // print all rxPower
                        file << ',';
                        for(i = 0; i < datalen; ++i){
                            file << rxPowerList[i];
                            if (i < datalen - 1){
                                file << ',';
                            }else{
                                file << '\n';
                            }
                        }
                        // print all bandwidth
                        file << ',';
                        for(i = 0; i < datalen; ++i){
                            file << bandwidthList[i];
                            if (i < datalen - 1){
                                file << ',';
                            }else{
                                file << '\n';
                            }
                        }
                        //release the memory
                        delete[] rxPowerList;
                        delete[] timeList;
                        delete[] bandwidthList;
                    }
                    // print - non-NNData
                    if(isNNData){
                        // print predicted mcs choice
                        file << this->macAddr << ',';
                        for(i = 0; i < __SDN_LAB_MCS_NUM; ++i){
                            file << this->nnMcsPredict[i];
                            if (i < __SDN_LAB_MCS_NUM - 1){
                                file << ',';
                            }else{
                                file << '\n';
                            }
                        }
                        // print predicted mcs activation time
                        file << ',';
                        for(i = 0; i < __SDN_LAB_MCS_NUM; ++i){
                            file << this->nnMcsActivateTime[i];
                            if (i < __SDN_LAB_MCS_NUM - 1){
                                file << ',';
                            }else{
                                file << '\n';
                            }
                        }
                    }   
                    // close the file
                    file.close();
                };
            #endif

            /*** Get & Set ***/
            // staDataListMaxLen
            unsigned int GetDataListMaxLen(){
                return this->datalistMaxLen;
            };
            // MacAddress (const before the parameter list means `this` is a const pointer and no change of its members is permitted)
            ns3::Mac48Address GetMacAddress() const{
                return this->macAddr;
            };
            // time
            void GetTimeList(double * list, unsigned int listMaxLen){
                GetDataList(list, __SDN_LAB_STATION_DATA_ITEM_TYPE_TIME, listMaxLen);
            };
            // rxPower
            void GetRxPowerList(double * list, unsigned int listMaxLen){
                GetDataList(list, __SDN_LAB_STATION_DATA_ITEM_TYPE_RXPOWER, listMaxLen);
            };
            // bandwidth
            void GetBandwidthList(unsigned int * list, unsigned int listMaxLen){
                GetDataList(list, __SDN_LAB_STATION_DATA_ITEM_TYPE_BANDWIDTH, listMaxLen);
            };
            // Minstrel-SNN
            // Minstrel-SNN+
            // Minstrel-AI
            // MCS & activate time
            void SetNNData(unsigned int * mcs, double * time, unsigned int datalen){
                // illegal inputs
                if(!(mcs && time)){
                    return;
                }
                // inputs check!
                unsigned int len = datalen < __SDN_LAB_MCS_NUM ? datalen : __SDN_LAB_MCS_NUM;
                unsigned int i = 0;
                // assign
                std::cout<<len<<std::endl;
                for(; i < len; ++i){
                    this->nnMcsPredict[i] = mcs[i];
                    this->nnMcsActivateTime[i] = time[i];
                }
                // pad 0s
                for(; i < __SDN_LAB_MCS_NUM; ++i){
                    this->nnMcsPredict[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                    this->nnMcsActivateTime[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                }
            };
            void GetNNData(unsigned int * mcs, double * time, unsigned int datalen = __SDN_LAB_MCS_NUM){
                // illegal inputs
                if(!(mcs && time)){
                    return;
                }
                // inputs check!
                unsigned int len = datalen < __SDN_LAB_MCS_NUM ? datalen : __SDN_LAB_MCS_NUM;
                unsigned int i = 0;
                // assign
                for(; i < len; ++i){
                    mcs[i] = this->nnMcsPredict[i];
                    time[i] = this->nnMcsActivateTime[i];
                }
                // pad 0s
                for(; i < datalen; ++i){
                    mcs[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                    time[i] = __SDN_LAB_NNDATA_ILLEGAL_DATA;
                }
            };

            /**
             * compaire whether two stations are the same
             * @sta1: the 1st station
             * @sta2: the 2nd station
             */
            friend bool operator == (const Station& sta1, const Station& sta2){
                return sta1.macAddr == sta2.macAddr;
            };
            /**
             * compaire whether the station is the address
             * @sta: the station
             * @addr: address
             */
            friend bool operator == (const Station& sta, const ns3::Mac48Address& addr){
                return sta.macAddr == addr;
            };
            friend bool operator == (const Station& sta, const char * addr){
                return sta.macAddr == addr;
            };
            friend bool operator == (const Station& sta, const std::string& addr){
                return sta.macAddr == addr.c_str();
            };
        };
        /*** redefined other relevant type names ***/
        typedef Station StationFactory; 
        typedef Station * PtrStation;
    }
#endif