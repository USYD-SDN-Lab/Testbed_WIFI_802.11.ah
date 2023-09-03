#pragma once
#ifndef __SDN_LAB_MCS_H
    #define __SDN_LAB_MCS_H
        // lowest MCS & highest MCS
        #define __SDN_LAB_MCS_LOWEST    10
        #define __SDN_LAB_MCS_HIGHEST   49
        // SDN Lab MCS
        #define __SDN_LAB_MCS_LEN 30
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

                static unsigned int Len(){
                    return __SDN_LAB_MCS_LEN;
                }
                static std::string GetModeNameAt(unsigned int i){
                    switch(i){
                        case 0:
                            return __SDN_LAB_MCS_110;
                        case 1:
                            return __SDN_LAB_MCS_10;
                        case 2:
                            return __SDN_LAB_MCS_11;
                        case 3:
                            return __SDN_LAB_MCS_12;
                        case 4:
                            return __SDN_LAB_MCS_13;
                        case 5:
                            return __SDN_LAB_MCS_14;
                        case 6:
                            return __SDN_LAB_MCS_15;
                        case 7:
                            return __SDN_LAB_MCS_16;
                        case 8:
                            return __SDN_LAB_MCS_17;
                        case 9:
                            return __SDN_LAB_MCS_18;
                        case 10:
                            return __SDN_LAB_MCS_19;
                        case 11:
                            return __SDN_LAB_MCS_20;
                        case 12:
                            return __SDN_LAB_MCS_21;
                        case 13:
                            return __SDN_LAB_MCS_22;
                        case 14:
                            return __SDN_LAB_MCS_23;
                        case 15:
                            return __SDN_LAB_MCS_24;
                        case 16:
                            return __SDN_LAB_MCS_25;
                        case 17:
                            return __SDN_LAB_MCS_26;
                        case 18:
                            return __SDN_LAB_MCS_27;
                        case 19:
                            return __SDN_LAB_MCS_28;
                        case 20:
                            return __SDN_LAB_MCS_40;
                        case 21:
                            return __SDN_LAB_MCS_41;
                        case 22:
                            return __SDN_LAB_MCS_42;
                        case 23:
                            return __SDN_LAB_MCS_43;
                        case 24:
                            return __SDN_LAB_MCS_44;
                        case 25:
                            return __SDN_LAB_MCS_45;
                        case 26:
                            return __SDN_LAB_MCS_46;
                        case 27:
                            return __SDN_LAB_MCS_47;
                        case 28:
                            return __SDN_LAB_MCS_48;
                        case 29:
                            return __SDN_LAB_MCS_49;
                    }
                    return __SDN_LAB_MCS_10;
                }
                static int GetDateRateAt(unsigned int i){
                    switch(i){
                        case 0:
                            //__SDN_LAB_MCS_110
                            return 150;
                        case 1:
                            //__SDN_LAB_MCS_10
                            return 300;
                        case 2:
                            //__SDN_LAB_MCS_11
                            return 600;
                        case 3:
                            //__SDN_LAB_MCS_12
                            return 900;
                        case 4:
                            //__SDN_LAB_MCS_13
                            return 1200;
                        case 5:
                            //__SDN_LAB_MCS_14
                            return 1800;
                        case 6:
                            //__SDN_LAB_MCS_15
                            return 2400;
                        case 7:
                            //__SDN_LAB_MCS_16
                            return 2700;
                        case 8:
                            //__SDN_LAB_MCS_17
                            return 3000;
                        case 9:
                            //__SDN_LAB_MCS_18
                            return 3600;
                        case 10:
                            //__SDN_LAB_MCS_19
                            return 4000;
                        case 11:
                            //__SDN_LAB_MCS_20
                            return 650;
                        case 12:
                            //__SDN_LAB_MCS_21
                            return 1300;
                        case 13:
                            //__SDN_LAB_MCS_22
                            return 1950;
                        case 14:
                            //__SDN_LAB_MCS_23
                            return 2600;
                        case 15:
                            //__SDN_LAB_MCS_24
                            return 3900;
                        case 16:
                            //__SDN_LAB_MCS_25
                            return 5200;
                        case 17:
                            //__SDN_LAB_MCS_26
                            return 5850;
                        case 18:
                            //__SDN_LAB_MCS_27
                            return 6500;
                        case 19:
                            //__SDN_LAB_MCS_28
                            return 7800;
                        case 20:
                            //__SDN_LAB_MCS_40
                            return 1350;
                        case 21:
                            //__SDN_LAB_MCS_41
                            return 2700;
                        case 22:
                            //__SDN_LAB_MCS_42
                            return 4050;
                        case 23:
                            //__SDN_LAB_MCS_43
                            return 5400;
                        case 24:
                            //__SDN_LAB_MCS_44
                            return 8100;
                        case 25:
                            //__SDN_LAB_MCS_45
                            return 10800;
                        case 26:
                            //__SDN_LAB_MCS_46
                            return 12150;
                        case 27:
                            //__SDN_LAB_MCS_47
                            return 13500;
                        case 28:
                            //__SDN_LAB_MCS_48
                            return 16200;
                        case 29:
                            //__SDN_LAB_MCS_49
                            return 18000;
                    }
                    return 0;
                }
            };
        }
#endif