#pragma once
#ifndef __PACKETCONTEXT_H
    #define __PACKETCONTEXT_H

    class PacketContext{
        private:
        double startTime;               // packet start time (sec)
        double endTime;                 // packet end time (sec)
        uint32_t bandwidth;             // the bandwidth (Hz)
        unsigned int mcs_in;            // the comming MCS (self-defined)
        unsigned int mcs_predict;       // the predicted MCS (self-defined)
        double per;                     // the probability error rate of the packet
        double snr;                     // SNR
        double rxPower;                 // Rx power in Watt
        double interferePower;          // the interference power
        
        
        public:
        /**
         * init
         * <INPUT>
         * @startTime:                  packet start time (sec)
         * @endTime:                    packet end time (sec)
         * @per:
         * @snr:
         * @rxPower:
         * @interferePower:
         * @modeName:                    the mode to calculate bandwidth & MCS
         */
        PacketContext(double startTime, double endTime, double per, double snr, double rxPower, double interferePower, std::string modeName){
            this->startTime = startTime;
            this->endTime = endTime;
            this->per = per;
            this->snr = snr;
            this->rxPower = rxPower;
            this->interferePower = interferePower;
        };
    };
#endif