#pragma once
#ifndef __SDN_LAB_OVERHEAD_SNN_H
    #define __SDN_LAB_OVERHEAD_SNN_H
    #include "ns3/mac48-address.h"      // support Mac48Address
    #include "Components/NNData.h"

    struct OverheadSNN{
        ns3::Mac48Address macAddr;
        unsigned int nnMcsPredict[__SDN_LAB_MCS_NUM];   // MCS
        double nnMcsActivateTime[__SDN_LAB_MCS_NUM];    // MCS activate time
    };
    typedef OverheadSNN * OverheadSNNList;
#endif