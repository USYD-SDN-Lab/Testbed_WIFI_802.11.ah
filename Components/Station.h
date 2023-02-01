#pragma once
#ifndef __STATION_H
    #define __STATION_H
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "StationData.h"
    class STA{
        private:
        /*** MEMBERS ***/
        ns3::Mac48Address macAddr;                      // the station mcs address
        unsigned int mcs_predict;                       // the predicted MCS
        unsigned int staDataListLen = 0;                // the station data length (default 0)
        unsigned int staDataListMaxLen;                 // the station data maximal length
        PtrStationData staDataList      = NULL;         // station data list (at the beginning node)
        PtrStationData staDataListEnd   = NULL;         // station data list end
        /*** METHODS ***/
        // clean the Station Data List
        void _ClearStationDataList(){
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

        public:
        /**
         * constructor
         * @macAddr:    the Mac Address
         */
        STA(ns3::Mac48Address macAddr){
            this->macAddr = macAddr;
        };
        /**
         * deconstructor
         */
        ~STA(){
            _ClearStationDataList();
        };


        /**
         * set the station data list length
         * <INPUT>
         * @memorySize: the extra memory can be allocated to each station (bytes)
         * <OUTPUT>
         * @other positive value:   the length of the station data list
         * @0:                      failure, the memory size is too small
         */
        unsigned int SetStaionDataListLen(unsigned int memorySize){
            this->staDataListMaxLen = (unsigned int)(memorySize / sizeof(StationData));
            return this->staDataListMaxLen
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
    };
#endif