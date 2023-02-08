#ifndef __SDN_LAB_STATIONDATA_H
    #define __SDN_LAB_STATIONDATA_H
    namespace SdnLab{
        /**
         * This class is to store the time-sequence data send from a station
         */
        typedef struct _StationData{
            double time;                    // the time point of this data
            double snr;                     // SNR
            double rxPower;                 // rxPower

            _StationData * next = NULL;
        }StationData, * PtrStationData;
    }
#endif