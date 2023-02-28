#pragma once
#ifndef __SDN_LAB_STATION_H
    #define __SDN_LAB_STATION_H
    #include <iostream>
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "Modules/Toolbox/Error.h"  // Error to throw
    // Memory Cost (base): 40
    // Memory Cost (data): 24
    namespace SdnLab{
        class Station{
            private:
            // private data structures (3*8 = 24 bytes)
            struct _Data{
                double time;        // the time point of this data
                double snr;         // SNR
                double rxPower;     // rxPower
            };
            // members
            ns3::Mac48Address macAddr;            // the station mcs address
            unsigned int mcs_predict;             // the predicted MCS
            unsigned int datalistLen = 0;         // the station data length
            unsigned int datalistMaxLen = 0;      // the station data maximal length
            _Data * datalist = NULL;              // station list
            unsigned int ptrb_datalist = 0;       // pointer -> station data list beginning
            unsigned int ptre_datalist = 0;       // pointer -> station data list end

            public:
            /**
             * constructor
             * @macAddr:    the Mac Address
             * @memorySize: the memory can be allocated to each station (bytes)
             */
            Station(ns3::Mac48Address macAddr, unsigned int memorySize){
                this->macAddr = macAddr;
                memorySize = memorySize - sizeof(Station);
                this->datalistMaxLen = (unsigned int)(memorySize / sizeof(_Data));
                try{
                    this->datalist = new _Data[this->datalistMaxLen];
                }catch(const std::bad_alloc & e){
                    Toolbox::Error err("/Components", "Station.h", "Station", "Station", "Cannot support that much data");
                    err.SetType2MemoryShortage();
                    throw err;
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

            /**
             * Summary the configuration
             */
            static void Summary(){
                std::cout << "SdnLab::Station " << std::endl;
                std::cout << " - Memory(base):" << sizeof(Station) << std::endl;
                std::cout << " - Memory(data):" << sizeof(_Data) << std::endl;
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
                // operate when the pointer is not null
                if(list){
                    unsigned int i;
                    unsigned int j = 0;
                    // init the list with 0
                    for(i = 0; i < listMaxLen; ++i){
                        list[i] = 0;
                    }
                    // append data into the list
                    if (this->ptrb_datalist <= this->ptre_datalist){
                        for(i = this->ptrb_datalist; i <= this->ptre_datalist; ++i){
                            list[j] = this->datalist[i].time;
                            ++j;
                        }
                    }
                    if (this->ptrb_datalist > this->ptre_datalist){
                        for(i = this->ptrb_datalist; i < this->datalistLen; ++i){
                            list[j] = this->datalist[i].time;
                            ++j;
                        }
                        for(i = 0; i <= this->ptre_datalist; ++i){
                            list[j] = this->datalist[i].time;
                            ++j;
                        }
                    }
                }
            }
            // rxPower
            void GetRxPowerList(double * list, unsigned int listMaxLen){
                // operate when the pointer is not null
                if(list){
                    unsigned int i;
                    unsigned int j = 0;
                    // init the list with 0
                    for(i = 0; i < listMaxLen; ++i){
                        list[i] = 0;
                    }
                    // append data into the list
                    if (this->ptrb_datalist <= this->ptre_datalist){
                        for(i = this->ptrb_datalist; i <= this->ptre_datalist; ++i){
                            list[j] = this->datalist[i].rxPower;
                            ++j;
                        }
                    }
                    if (this->ptrb_datalist > this->ptre_datalist){
                        for(i = this->ptrb_datalist; i < this->datalistLen; ++i){
                            list[j] = this->datalist[i].rxPower;
                            ++j;
                        }
                        for(i = 0; i <= this->ptre_datalist; ++i){
                            list[j] = this->datalist[i].rxPower;
                            ++j;
                        }
                    }
                }
            }
        };
        /*** redefined other relevant type names ***/
        typedef Station * PtrStation;
    }
#endif