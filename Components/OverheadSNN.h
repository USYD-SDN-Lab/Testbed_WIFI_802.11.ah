#pragma once
#ifndef __SDN_LAB_OVERHEAD_SNN_H
    #define __SDN_LAB_OVERHEAD_SNN_H
    #define __SDN_LAB_OVERHEAD_SNN_ITEM_DATALEN 29
    #include "Modules/Toolbox/Error.h"          // Error to throw
    #include "ns3/mac48-address.h"              // support Mac48Address
    #include "Mac.h"
    #include "StationList.h"
    #include "Overhead.h"                       // general overhead
    namespace SdnLab{
        struct OverheadSNNItem{
            ns3::Mac48Address macAddr = __SDN_LAB_MAC_BROADCAST_ADDR;
            unsigned int nnMcsPredict[__SDN_LAB_OVERHEAD_SNN_ITEM_DATALEN];   // MCS
            double nnMcsActivateTime[__SDN_LAB_OVERHEAD_SNN_ITEM_DATALEN];    // MCS activate time
            /**
             * match to a MAC address
             * @addr: the MAC address to compare with
             */
            bool Match(ns3::Mac48Address addr){
                return this->macAddr == addr;
            };
        };

        class OverheadSNN : public Overhead{
            private:
                unsigned int len = 0;
                OverheadSNNItem * datalist = NULL;
                
                /**
                 * constructor
                 */
                OverheadSNN(unsigned int len, OverheadSNNItem * datalist){
                    unsigned int i, j;
                    this->len = len;
                    this->datalist = datalist;
                    if(this->len != 0 && this->datalist){
                        this->datalist = new OverheadSNNItem[this->len];
                        for(i = 0; i < this->len; ++i){
                            this->datalist[i].macAddr = datalist[i].macAddr;
                            for(j = 0; j < __SDN_LAB_OVERHEAD_SNN_ITEM_DATALEN; ++j){
                                this->datalist[i].nnMcsPredict[j] = datalist[i].nnMcsPredict[j];
                                this->datalist[i].nnMcsActivateTime[j] = datalist[i].nnMcsActivateTime[j];
                            }
                        }
                    }
                };
                OverheadSNN(StationList list){
                    unsigned int i;
                    if(list){
                        this->len = list->GetLen();
                        if(this->len != 0){
                            this->datalist = new OverheadSNNItem[this->len];
                            // assign data to it
                            auto iter = list->Begin();
                            for(i=0; i<this->len; ++iter, ++i){
                                if(iter){
                                    (*iter)->GetNNData(this->datalist[i].nnMcsPredict, this->datalist[i].nnMcsActivateTime, __SDN_LAB_OVERHEAD_SNN_ITEM_DATALEN);
                                    this->datalist[i].macAddr = (*iter)->GetMacAddress();
                                }else{
                                    // only happens when the list is empty
                                    break;
                                }
                            }
                        }
                    }
                };

            public:
                /**
                 * copy
                 */
                OverheadSNN * Copy(){
                    OverheadSNN * overhead = NULL;
                    try{
                        overhead = new OverheadSNN(this->len, this->datalist);
                    }catch(const std::bad_alloc & e){
                        Toolbox::Error err("/Components", "OverheadSNN.h", "OverheadSNN", "Copy", __SDN_LAB_OVERHEAD_MSG_MEMORY_SHORTAGE);
                        err.SetType2MemoryShortage();
                        throw err;
                    }
                    return overhead;
                };

                /** 
                 * clear the memory
                 */
                void Clear(){
                    if(this->datalist){
                        delete[] this->datalist;
                    }
                    this->len = 0;
                };

                /**
                 * create
                 * @list: the list of stations
                 */
                static OverheadSNN * Create(StationList list){
                    OverheadSNN * overhead = NULL;
                    if(list){
                        // create an overhead
                        try{
                            overhead = new OverheadSNN(list);
                        }catch(const std::bad_alloc & e){
                            Toolbox::Error err("/Components", "OverheadSNN.h", "OverheadSNN", "Create", __SDN_LAB_OVERHEAD_MSG_MEMORY_SHORTAGE);
                            err.SetType2MemoryShortage();
                            throw err;
                        }
                    }
                    return overhead;
                };

                /*** Get & Set ***/
                unsigned int GetLen(){
                    return this->len;
                }

                /*** iterations ***/
                OverheadSNNItem * Begin(){
                    return this->datalist;
                };
                OverheadSNNItem * End(){
                    if(this->len != 0){
                        return (this->datalist + this->len - 1);
                    }else{
                        return this->datalist;
                    }
                };
        };
    }
#endif