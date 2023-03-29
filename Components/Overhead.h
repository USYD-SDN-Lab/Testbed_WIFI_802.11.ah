#pragma once
#ifndef __SDN_LAB_OVERHEAD_H
    #define __SDN_LAB_OVERHEAD_H
    #define __SDN_LAB_OVERHEAD_MSG_MEMORY_SHORTAGE "no memory to create an overhead"
    namespace SdnLab{
        class Overhead{
            protected:
                Overhead(){};
                ~Overhead(){};
            public:
                // copy a new overhead
                virtual Overhead * Copy() = 0;

                // clear all assigned memory
                virtual void Clear(){};

                // destroy
                static void Destroy(Overhead * overhead){
                    // clear memory
                    if(overhead){
                        overhead->Clear();
                        delete overhead;
                    }
                };
        };
    }
#endif