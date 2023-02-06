#pragma once
#ifndef __SDN_LAB_STATION_H
    #define __SDN_LAB_STATION_H
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "StationData.h"
    class Station{
        private:
        ns3::Mac48Address macAddr;                      // the station mcs address
        unsigned int mcs_predict;                       // the predicted MCS
        unsigned int staDataListLen = 0;                // the station data length (default 0)
        unsigned int staDataListMaxLen = 0;             // the station data maximal length
        PtrStationData staDataList      = NULL;         // station data list (at the beginning node)
        PtrStationData staDataListEnd   = NULL;         // station data list end
    
        public:
        /**
         * constructor
         * @macAddr:    the Mac Address
         * @memorySize: the memory can be allocated to each station (bytes)
         */
        Station(ns3::Mac48Address macAddr, unsigned int memorySize){
            this->macAddr = macAddr;
            this->staDataListMaxLen = (unsigned int)(memorySize / sizeof(StationData));
        };
        /**
         * deconstructor
         */
        ~Station(){
            Clear();
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
        bool AddStationData(double time, double snr, double rxPower){
            // create
            PtrStationData staData = new StationData;
            staData->time = time;
            staData->snr = snr;
            staData->rxPower = rxPower;
            // check whether there is enough room to add
            if (this->staDataListLen < this->staDataListMaxLen){
                // add
                if(this->staDataList && this->staDataListEnd){
                    // with other elements
                    this->staDataListEnd->next = staData;
                    this->staDataListEnd = staData;
                }else{
                    // with 0 element
                    this->staDataList = staData;
                    this->staDataListEnd = staData;
                }
                // counter ++
                this->staDataListLen++;
                return true;
            }else{
                return false;
            }
        }

        /**
         * clear the Station Data List
         */
        void Clear(){
            // release memory
            PtrStationData cur = this->staDataList;
            PtrStationData next = NULL;
            while(cur){
                // record the next one
                next = cur->next;
                // release the current memory
                delete cur;
                // move to the next
                cur = next;
            }
            // reset pointers to NULL
            this->staDataList = NULL;
            this->staDataListEnd = NULL;
            // counter set to 0
            this->staDataListLen = 0;
        };

        /**
         * compaire whether two stations are the same
         * @sta1: the 1st station
         * @sta2: the 2nd station
         */
        friend bool operator == (const Station& sta1, const Station& sta2){
            return sta1.macAddr == sta2.macAddr;
        }

        /*** Get & Set ***/
        // staDataListMaxLen
        unsigned int GetStaionDataListMaxLen(){
            return this->staDataListMaxLen;
        }
        // MacAddress (const before the parameter list means `this` is a const pointer and no change of its members is permitted)
        ns3::Mac48Address GetMacAddress() const{
            return this->macAddr;
        }
    };
    /*** redefined other relevant type names ***/
    typedef Station * PtrStation;
#endif