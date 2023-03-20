#pragma once
#ifndef __SDN_LAB_STATION_H
    #define __SDN_LAB_STATION_H
    #include <iostream>
    #include <climits>                  // support `UINT_MAX`
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "Modules/Toolbox/Error.h"  // Error to throw
    #define __SDN_LAB_STATION_MEMORY_COST_BASE              40      // Memory Cost (base): 40
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
                unsigned int bandwidth;             // bandwidth
            };
            /*** members ***/
            ns3::Mac48Address macAddr;              // the station mcs address
            unsigned int mcs_predict = 10;          // the predicted MCS (use the lowest MCS)
            unsigned int datalistLen = 0;           // the station data length
            unsigned int datalistMaxLen = 0;        // the station data maximal length
            _Data * datalist = NULL;                // station list
            unsigned int ptrb_datalist = 0;         // pointer -> station data list beginning
            unsigned int ptre_datalist = 0;         // pointer -> station data list end
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
                    for(; j >= this->datalistLen; --j){
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
             * <OUTPUT>
             * bool, whether success or failure
             */
            void AddData(double time, double snr, double rxPower){
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
            };

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
                void Summary2File(std::string & filepath){
                    std::fstream file;
                    unsigned int i;
                    // retrieve the time list & rxPower list
                    double * timeList = new double[this->datalistMaxLen];
                    double * rxPowerList = new double[this->datalistMaxLen];
                    std::cout<<"  Going to retrieve, datalistmaxlen = " << this->datalistMaxLen << '\n';
                    GetTimeList(timeList, this->datalistMaxLen);
                    GetRxPowerList(rxPowerList, this->datalistMaxLen);
                    // open the file
                    file.open(filepath, std::fstream::in | std::fstream::app);
                    file << this->macAddr << ',';
                    // print all time
                    for(i = 0; i < this->datalistMaxLen; ++i){
                        file << timeList[i];
                        if (i < this->datalistMaxLen - 1){
                            file << ',';
                        }else{
                            file << '\n';
                        }
                    }
                    file << ',';
                    // print all rxPower
                    for(i = 0; i < this->datalistMaxLen; ++i){
                        file << rxPowerList[i];
                        if (i < this->datalistMaxLen - 1){
                            file << ',';
                        }else{
                            file << '\n';
                        }
                    }
                    file.close();
                    //release the memory
                    delete[] rxPowerList;
                    delete[] timeList;
                }
            #endif

            

            /*** Get & Set ***/
            // staDataListMaxLen
            unsigned int GetDataListMaxLen(){
                return this->datalistMaxLen;
            };
            // MacAddress (const before the parameter list means `this` is a const pointer and no change of its members is permitted)
            #ifdef __SDN_LAB_DEBUG
                ns3::Mac48Address GetMacAddress() const{
                    return this->macAddr;
                };
            #endif
            // time
            void GetTimeList(double * list, unsigned int listMaxLen){
                GetDataList(list, __SDN_LAB_STATION_DATA_ITEM_TYPE_TIME, listMaxLen);
            }
            // rxPower
            void GetRxPowerList(double * list, unsigned int listMaxLen){
                GetDataList(list, __SDN_LAB_STATION_DATA_ITEM_TYPE_RXPOWER, listMaxLen);
            }
            // bandwidth
            void GetBandwidthList(unsigned int * list, unsigned int listMaxLen){
                GetDataList(list, __SDN_LAB_STATION_DATA_ITEM_TYPE_BANDWIDTH, listMaxLen);
            }
            // mcs_predict
            void SetMcsPredict(unsigned int mcs){
                this->mcs_predict = mcs;
            }
            unsigned int GetMcsPredict(){
                return this->mcs_predict;
            }

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