#pragma once
#ifndef __PACKETCONTEXT_H
    #define __PACKETCONTEXT_H
    #include "ns3/mac48-address.h"      // support Mac48Address

    class PacketContext{
        private:
        ns3::Mac48Address txMacAddr;    // Tx address
        ns3::Mac48Address rxMacAddr;    // Rx address
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
         * retrieve the bandwidth from the mode name
         */
        static uint32_t ModeName2Bandwidth(std::string modeName){
            uint32_t bandwidth = 0;
            if (modeName == "OfdmRate300KbpsBW1MHz" ||
                modeName == "OfdmRate600KbpsBW1MHz" ||
                modeName == "OfdmRate900KbpsBW1MHz" ||
                modeName == "OfdmRate1_2MbpsBW1MHz" ||
                modeName == "OfdmRate1_8MbpsBW1MHz" ||
                modeName == "OfdmRate2_4MbpsBW1MHz" ||
                modeName == "OfdmRate2_7MbpsBW1MHz" ||
                modeName == "OfdmRate3MbpsBW1MHz"   ||
                modeName == "OfdmRate3_6MbpsBW1MHz" ||
                modeName == "OfdmRate4MbpsBW1MHz"   ||
                modeName == "OfdmRate150KbpsBW1MHz"){
                bandwidth = 1;
            }else if (modeName == "OfdmRate650KbpsBW2MHz"  ||
                      modeName == "OfdmRate1_3MbpsBW2MHz"  ||
                      modeName == "OfdmRate1_95MbpsBW2MHz" ||
                      modeName == "OfdmRate2_6MbpsBW2MHz"  ||
                      modeName == "OfdmRate3_9MbpsBW2MHz"  ||
                      modeName == "OfdmRate5_2MbpsBW2MHz"  ||
                      modeName == "OfdmRate5_85MbpsBW2MHz" ||
                      modeName == "OfdmRate6_5MbpsBW2MHz"  ||
                      modeName == "OfdmRate7_8MbpsBW2MHz"){
                bandwidth = 2;
            }else if (modeName == "OfdmRate1_35MbpsBW4MHz" ||
                      modeName == "OfdmRate2_7MbpsBW4MHz"  ||
                      modeName == "OfdmRate4_05MbpsBW4MHz" ||
                      modeName == "OfdmRate5_4MbpsBW4MHz"  ||
                      modeName == "OfdmRate8_1MbpsBW4MHz"  ||
                      modeName == "OfdmRate10_8MbpsBW4MHz" ||
                      modeName == "OfdmRate12_15MbpsBW4MHz"||
                      modeName == "OfdmRate13_5MbpsBW4MHz" ||
                      modeName == "OfdmRate16_2MbpsBW4MHz" ||
                      modeName == "OfdmRate18MbpsBW4MHz"){
                bandwidth = 4;
            }
            return bandwidth;
        };
        /**
         * retrieve MCS from the mode name
         */
        static unsigned int ModeName2MCS(std::string modeName){
            unsigned int mcs = 0;
            if (modeName == "OfdmRate300KbpsBW1MHz"){
                mcs = 10;
            }else if(modeName == "OfdmRate600KbpsBW1MHz"){
                mcs = 11;
            }else if(modeName == "OfdmRate900KbpsBW1MHz"){
                mcs = 12;
            }else if(modeName == "OfdmRate1_2MbpsBW1MHz"){
                mcs = 13;
            }else if(modeName == "OfdmRate1_8MbpsBW1MHz"){
                mcs = 14;
            }else if(modeName == "OfdmRate2_4MbpsBW1MHz"){
                mcs = 15;
            }else if(modeName == "OfdmRate2_7MbpsBW1MHz"){
                mcs = 16;
            }else if(modeName == "OfdmRate3MbpsBW1MHz"){
                mcs = 17;
            }else if(modeName == "OfdmRate3_6MbpsBW1MHz"){
                mcs = 18;
            }else if(modeName == "OfdmRate4MbpsBW1MHz"){
                mcs = 19;
            }else if(modeName == "OfdmRate150KbpsBW1MHz"){
                mcs = 110;
            }else if(modeName == "OfdmRate650KbpsBW2MHz"){
                mcs = 20;
            }else if(modeName == "OfdmRate1_3MbpsBW2MHz"){
                mcs = 21;
            }else if(modeName == "OfdmRate1_95MbpsBW2MHz"){
                mcs = 22;
            }else if(modeName == "OfdmRate2_6MbpsBW2MHz"){
                mcs = 23;
            }else if(modeName == "OfdmRate3_9MbpsBW2MHz"){
                mcs = 24;
            }else if(modeName == "OfdmRate5_2MbpsBW2MHz"){
                mcs = 25;
            }else if(modeName == "OfdmRate5_85MbpsBW2MHz"){
                mcs = 26;
            }else if(modeName == "OfdmRate6_5MbpsBW2MHz"){
                mcs = 27;
            }else if(modeName == "OfdmRate7_8MbpsBW2MHz"){
                mcs = 28;
            }else if(modeName == "OfdmRate1_35MbpsBW4MHz"){
                mcs = 40;
            }else if(modeName == "OfdmRate2_7MbpsBW4MHz"){
                mcs = 41;
            }else if(modeName == "OfdmRate4_05MbpsBW4MHz"){
                mcs = 42;
            }else if(modeName == "OfdmRate5_4MbpsBW4MHz"){
                mcs = 43;
            }else if(modeName == "OfdmRate8_1MbpsBW4MHz"){
                mcs = 44;
            }else if(modeName == "OfdmRate10_8MbpsBW4MHz"){
                mcs = 45;
            }else if(modeName == "OfdmRate12_15MbpsBW4MHz"){
                mcs = 46;
            }else if(modeName == "OfdmRate13_5MbpsBW4MHz"){
                mcs = 47;
            }else if(modeName == "OfdmRate16_2MbpsBW4MHz"){
                mcs = 48;
            }else if(modeName == "OfdmRate18MbpsBW4MHz"){
                mcs = 49;
            }
            return mcs;
        };
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
            // calculate bandwidth & mcs_in from the mode name
            this->bandwidth = PacketContext::ModeName2Bandwidth(modeName);
            this->mcs_in = PacketContext::ModeName2MCS(modeName);
        };

        /**
         * set the Tx & Rx Mac address
         */
        void SetTxRxMacAddr(){

        }
    };
#endif