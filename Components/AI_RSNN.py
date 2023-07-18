import sys 
sys.path.append("..");
import os
cur_path = os.getcwd();
os.chdir("./Components");
import numpy as np
import gc
import math
from py_interface import *
from Modules.RA_Minstrel_SNN.RA_Minstrel_SNN import RA_Minstrel_SNN;
from NNData import memory_pool_size, _SDN_LAB_NNDATA_MEMORY_ID, _SDN_LAB_NNDATA_LEN, _SDN_LAB_MCS_NUM, _SDN_LAB_NNDATA_ILLEGAL_DATA, NNFeature, NNPredicted, NNTarget
from Settings import Settings

os.chdir(cur_path);

# load settings
settings = Settings();
# init the meory pool with <id, pool_size>
# the memory pool id 1234 is not used in C/C++, we can define any number we want
Init(1234, memory_pool_size);
# Link the shared memory block with ns-3 script
dl = Ns3AIDL(_SDN_LAB_NNDATA_MEMORY_ID, NNFeature, NNPredicted, NNTarget);
# load the SNN
print(settings.PathSNN());
rms = RA_Minstrel_SNN(snn_model_prefix = settings.PathSNN());
rmr = RA_Minstrel_SNN(snn_model_prefix = settings.PathRNN());

# noise settings
BOLTZMANN = 1.3803e-23;                     # Boltzmann constants
tempature = 290;                            # temperature is 290K at 15 degree Centigrade
noiseFigure = 4.7863;                       # amplify times due to non-idealities of Rx
No = BOLTZMANN*tempature*noiseFigure;       # noise density
ch_bws = [4000000, 2000000, 1000000];       # the channel bandwidths

# retrieve data
try:
    while True:
        with dl as data:
            print("WAITING FOR DATA");
            if dl.isFinish() or data == None:
                break;
            gc.collect();
            # we set all data is illegal
            # if the input is illegal, we do nothing
            is_all_rssi_assigned = True;
            for i in range(_SDN_LAB_NNDATA_LEN - 1, -1, -1):
                if data.feat.rxPower[i] == _SDN_LAB_NNDATA_ILLEGAL_DATA:
                    is_all_rssi_assigned = False;
                    break;

            if not is_all_rssi_assigned:
                print("  - Not all RSSI are assigned, pass");
                pass
            # we retrieve all RSSI
            else:
                # predict the future RSSI
                


                lastRxPower = _SDN_LAB_NNDATA_ILLEGAL_DATA;
                lastBandwidth = _SDN_LAB_NNDATA_ILLEGAL_DATA;
                for i in range(_SDN_LAB_NNDATA_LEN - 1, -1, -1):
                    if data.feat.rxPower[i] != _SDN_LAB_NNDATA_ILLEGAL_DATA:
                        lastRxPower = data.feat.rxPower[i];
                        lastBandwidth = data.feat.bandwidth[i];
                        break;
                lastSNR = lastRxPower/(No*lastBandwidth);
                # predict mcs
                data.pred.mcs[0] = rms.predict(lastSNR);
                # print
                print('  - SNR=%.4f, MCS=%d'%(lastSNR, data.pred.mcs[0]), end="");
                # release temporay variables
                del lastRxPower, lastBandwidth, lastSNR;
            print();
except KeyboardInterrupt:
    print('Ctrl C')
except Exception as e:
    print('Something wrong')
    print(e)
finally:
    FreeMemory()