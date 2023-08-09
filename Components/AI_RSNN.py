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
from Modules.RA_Minstrel_RNN.RA_Minstrel_RNN import RA_Minstrel_RNN;
from NNData import memory_pool_size, _SDN_LAB_NNDATA_MEMORY_ID, _SDN_LAB_NNDATA_LEN, _SDN_LAB_MCS_NUM, _SDN_LAB_NNDATA_ILLEGAL_DATA, NNFeature, NNPredicted, NNTarget
from Settings import Settings

os.chdir(cur_path);

# load settings
settings = Settings();
model_memory_len = 6;
is_dbm = True;
# load the SNN
rms = RA_Minstrel_SNN(snn_model_prefix = settings.PathSNN());
rmr = RA_Minstrel_RNN(model_prefix = settings.PathRNN(), model_memory_len=model_memory_len);

# noise settings
BOLTZMANN = 1.3803e-23;                                 # Boltzmann constants
tempature = 290;                                        # temperature is 290K at 15 degree Centigrade
noiseFigure = 4.7863;                                   # amplify times due to non-idealities of Rx
No = BOLTZMANN*tempature*noiseFigure;                   # noise density
ch_bws = np.array([4000000, 2000000, 1000000]);         # the channel bandwidths

# init the meory pool with <id, pool_size>
# the memory pool id 1234 is not used in C/C++, we can define any number we want
Init(1234, memory_pool_size);
# Link the shared memory block with ns-3 script
dl = Ns3AIDL(_SDN_LAB_NNDATA_MEMORY_ID, NNFeature, NNPredicted, NNTarget);
# retrieve data
try:
    while True:
        with dl as data:
            print("WAITING FOR DATA");
            if dl.isFinish() or data == None:
                break;
            gc.collect();
            
            # check whether we have data
            is_enough_data = False;
            data_last_index = 0;
            for i in range(_SDN_LAB_NNDATA_ILLEGAL_DATA-1, -1, -1):
                # now we have the latest data
                if data.feat.rxPower[i] != _SDN_LAB_NNDATA_ILLEGAL_DATA:
                    # if we have data, then check wether the length is enough
                    if i >= model_memory_len - 1:
                        is_enough_data = True;
                        data_last_index = i;
                    break;
            
            # no data
            if not is_enough_data:
                print("  - there is not enough data, pass");
                for i in range(_SDN_LAB_NNDATA_ILLEGAL_DATA):
                    print(data.feat.rxPower[i], end=", ");
                print();
                continue;

            # enough data
            data_start_index = data_last_index - model_memory_len + 1;
            data_adjust = [];
            for i in range(data_start_index, data_last_index + 1):
                data_adjust.append(data.feat.rxPower[i]);
            # fill the missing data with neighbour value - forward filling
            data_previous = data_adjust[0];
            for i in range(len(data_adjust)):
                if data_adjust[i] == _SDN_LAB_NNDATA_ILLEGAL_DATA:
                    data_adjust[i] = data_previous;
                else:
                    data_previous = data_adjust[i];
            # fill the missing data with neighbour value - backward filling
            data_posterior = data_adjust[-1];
            for i in range(len(data_adjust)-1, -1, -1):
                if data_adjust[i] == _SDN_LAB_NNDATA_ILLEGAL_DATA:
                    data_adjust[i] = data_posterior;
                else:
                    data_posterior = data_adjust[i];
            # unit transform
            data_adjust = np.asarray(data_adjust);
            if is_dbm:
                data_adjust = 10*np.log10(data_adjust) + 30;
            # predict future RSSI
            rssi_future = rmr.predict(data_adjust);
            # calculate future SNR
            snr_future = rssi_future/(No*ch_bws);
            # predict MCS
            tmp_mcs = 0;
            tmp_mcs = rms.predict(snr_future[0]);
            if tmp_mcs < 40:
                tmp_mcs = rms.predict(snr_future[1]);
            elif tmp_mcs < 20:
                tmp_mcs = rms.predict(snr_future[2]);
            data.pred.mcs[0] = tmp_mcs;
            # print
            print('  - SNR=%.4f, MCS=%d'%(snr_future, data.pred.mcs[0]));
            # release temporay variables
            del lastRxPower, lastBandwidth, lastSNR;
except KeyboardInterrupt:
    print('Ctrl C')
except Exception as e:
    print('Something wrong')
    print(e)
finally:
    FreeMemory()