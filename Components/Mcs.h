#pragma once
#ifndef __SDN_LAB_MCS_H
    #define __SDN_LAB_MCS_H
        // lowest MCS & highest MCS
        #define __SDN_LAB_MCS_LOWEST    10
        #define __SDN_LAB_MCS_HIGHEST   49
        // SDN Lab MCS
        #define __SDN_LAB_MCS_110 "OfdmRate150KbpsBW1MHz"
        #define __SDN_LAB_MCS_10  "OfdmRate300KbpsBW1MHz"
        #define __SDN_LAB_MCS_11  "OfdmRate600KbpsBW1MHz"
        #define __SDN_LAB_MCS_12  "OfdmRate900KbpsBW1MHz"
        #define __SDN_LAB_MCS_13  "OfdmRate1_2MbpsBW1MHz"
        #define __SDN_LAB_MCS_14  "OfdmRate1_8MbpsBW1MHz"
        #define __SDN_LAB_MCS_15  "OfdmRate2_4MbpsBW1MHz"
        #define __SDN_LAB_MCS_16  "OfdmRate2_7MbpsBW1MHz"
        #define __SDN_LAB_MCS_17  "OfdmRate3MbpsBW1MHz"
        #define __SDN_LAB_MCS_18  "OfdmRate3_6MbpsBW1MHz"
        #define __SDN_LAB_MCS_19  "OfdmRate4MbpsBW1MHz"
        #define __SDN_LAB_MCS_20  "OfdmRate650KbpsBW2MHz"
        #define __SDN_LAB_MCS_21  "OfdmRate1_3MbpsBW2MHz"
        #define __SDN_LAB_MCS_22  "OfdmRate1_95MbpsBW2MHz"
        #define __SDN_LAB_MCS_23  "OfdmRate2_6MbpsBW2MHz"
        #define __SDN_LAB_MCS_24  "OfdmRate3_9MbpsBW2MHz"
        #define __SDN_LAB_MCS_25  "OfdmRate5_2MbpsBW2MHz"
        #define __SDN_LAB_MCS_26  "OfdmRate5_85MbpsBW2MHz"
        #define __SDN_LAB_MCS_27  "OfdmRate6_5MbpsBW2MHz"
        #define __SDN_LAB_MCS_28  "OfdmRate7_8MbpsBW2MHz"
        #define __SDN_LAB_MCS_29                            // not defined in ns3
        #define __SDN_LAB_MCS_40  "OfdmRate1_35MbpsBW4MHz"
        #define __SDN_LAB_MCS_41  "OfdmRate2_7MbpsBW4MHz"
        #define __SDN_LAB_MCS_42  "OfdmRate4_05MbpsBW4MHz"
        #define __SDN_LAB_MCS_43  "OfdmRate5_4MbpsBW4MHz"
        #define __SDN_LAB_MCS_44  "OfdmRate8_1MbpsBW4MHz"
        #define __SDN_LAB_MCS_45  "OfdmRate10_8MbpsBW4MHz"
        #define __SDN_LAB_MCS_46  "OfdmRate12_15MbpsBW4MHz"
        #define __SDN_LAB_MCS_47  "OfdmRate13_5MbpsBW4MHz"
        #define __SDN_LAB_MCS_48  "OfdmRate16_2MbpsBW4MHz"
        #define __SDN_LAB_MCS_49  "OfdmRate18MbpsBW4MHz"
        // SDN Lab MCS - Vincent
        #define __SDN_LAB_MCS_10_TO_VINCENT     0
        #define __SDN_LAB_MCS_11_TO_VINCENT     1
        #define __SDN_LAB_MCS_12_TO_VINCENT     2
        #define __SDN_LAB_MCS_13_TO_VINCENT     3
        #define __SDN_LAB_MCS_14_TO_VINCENT     4
        #define __SDN_LAB_MCS_15_TO_VINCENT     5
        #define __SDN_LAB_MCS_16_TO_VINCENT     6
        #define __SDN_LAB_MCS_17_TO_VINCENT     7
        #define __SDN_LAB_MCS_18_TO_VINCENT     8
        #define __SDN_LAB_MCS_19_TO_VINCENT     9
        #define __SDN_LAB_MCS_110_TO_VINCENT                // not defined by Vincent
        #define __SDN_LAB_MCS_20_TO_VINCENT     10
        #define __SDN_LAB_MCS_21_TO_VINCENT     11
        #define __SDN_LAB_MCS_22_TO_VINCENT     12
        #define __SDN_LAB_MCS_23_TO_VINCENT     13
        #define __SDN_LAB_MCS_24_TO_VINCENT     14
        #define __SDN_LAB_MCS_25_TO_VINCENT     15
        #define __SDN_LAB_MCS_26_TO_VINCENT     16
        #define __SDN_LAB_MCS_27_TO_VINCENT     17
        #define __SDN_LAB_MCS_28_TO_VINCENT     18
        #define __SDN_LAB_MCS_29_TO_VINCENT                 // not defined in ns3
        #define __SDN_LAB_MCS_40_TO_VINCENT     19
        #define __SDN_LAB_MCS_41_TO_VINCENT     20
        #define __SDN_LAB_MCS_42_TO_VINCENT     21
        #define __SDN_LAB_MCS_43_TO_VINCENT     22
        #define __SDN_LAB_MCS_44_TO_VINCENT     23
        #define __SDN_LAB_MCS_45_TO_VINCENT     24
        #define __SDN_LAB_MCS_46_TO_VINCENT     25
        #define __SDN_LAB_MCS_47_TO_VINCENT     26
        #define __SDN_LAB_MCS_48_TO_VINCENT     27
        #define __SDN_LAB_MCS_49_TO_VINCENT     28
        namespace SdnLab{
            class Mcs{
                public:
                static unsigned int FromModeName(std::string modeName){
                    unsigned int mcs = 0;
                    if (modeName == __SDN_LAB_MCS_10){
                        mcs = 10;
                    }else if(modeName == __SDN_LAB_MCS_11){
                        mcs = 11;
                    }else if(modeName == __SDN_LAB_MCS_12){
                        mcs = 12;
                    }else if(modeName == __SDN_LAB_MCS_13){
                        mcs = 13;
                    }else if(modeName == __SDN_LAB_MCS_14){
                        mcs = 14;
                    }else if(modeName == __SDN_LAB_MCS_15){
                        mcs = 15;
                    }else if(modeName == __SDN_LAB_MCS_16){
                        mcs = 16;
                    }else if(modeName == __SDN_LAB_MCS_17){
                        mcs = 17;
                    }else if(modeName == __SDN_LAB_MCS_18){
                        mcs = 18;
                    }else if(modeName == __SDN_LAB_MCS_19){
                        mcs = 19;
                    }else if(modeName == __SDN_LAB_MCS_110){
                        mcs = 110;
                    }else if(modeName == __SDN_LAB_MCS_20){
                        mcs = 20;
                    }else if(modeName == __SDN_LAB_MCS_21){
                        mcs = 21;
                    }else if(modeName == __SDN_LAB_MCS_22){
                        mcs = 22;
                    }else if(modeName == __SDN_LAB_MCS_23){
                        mcs = 23;
                    }else if(modeName == __SDN_LAB_MCS_24){
                        mcs = 24;
                    }else if(modeName == __SDN_LAB_MCS_25){
                        mcs = 25;
                    }else if(modeName == __SDN_LAB_MCS_26){
                        mcs = 26;
                    }else if(modeName == __SDN_LAB_MCS_27){
                        mcs = 27;
                    }else if(modeName == __SDN_LAB_MCS_28){
                        mcs = 28;
                    }else if(modeName == __SDN_LAB_MCS_40){
                        mcs = 40;
                    }else if(modeName == __SDN_LAB_MCS_41){
                        mcs = 41;
                    }else if(modeName == __SDN_LAB_MCS_42){
                        mcs = 42;
                    }else if(modeName == __SDN_LAB_MCS_43){
                        mcs = 43;
                    }else if(modeName == __SDN_LAB_MCS_44){
                        mcs = 44;
                    }else if(modeName == __SDN_LAB_MCS_45){
                        mcs = 45;
                    }else if(modeName == __SDN_LAB_MCS_46){
                        mcs = 46;
                    }else if(modeName == __SDN_LAB_MCS_47){
                        mcs = 47;
                    }else if(modeName == __SDN_LAB_MCS_48){
                        mcs = 48;
                    }else if(modeName == __SDN_LAB_MCS_49){
                        mcs = 49;
                    }
                    return mcs;
                }
            };
        }
#endif