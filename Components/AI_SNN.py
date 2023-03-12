import sys 
sys.path.append("..");
import os
cur_path = os.getcwd();
os.chdir("./Components");
import numpy as np
import math
from py_interface import *
from Modules.RA_Minstrel_SNN.RA_Minstrel_SNN import RA_Minstrel_SNN;
from Mcs import _SDN_LAB_MCS_NUM
from NNData import memory_pool_size, _SDN_LAB_NNDATA_MEMORY_ID, NNFeature, NNPredicted, NNTarget
from Settings import Settings

# load settings
settings = Settings();
# init the meory pool with <id, pool_size>
# the memory pool id 1234 is not used in C/C++, we can define any number we want
Init(1234, memory_pool_size);
# Link the shared memory block with ns-3 script
dl = Ns3AIDL(_SDN_LAB_NNDATA_MEMORY_ID, NNFeature, NNPredicted, NNTarget);
# load the SNN
rms = RA_Minstrel_SNN(snn_model_prefix = settings.PathSNN());

try:
    while True:
        with dl as data:
            print("WAITING FOR DATA")
            if data == None:
                break
            # print('data.feat.wbCqi', data.feat.wbCqi)
            # Deep Learning code there

            #signal = -math.log10((snr/1.98)*(3.83*pow(10,-14)))
            rxPower = data.feat.rxPower;
            
            sinr_log = np.array([np.log(sinr)])
            mcs_rate = 0;
            

            print("sinr:", sinr, "sinr_log:", sinr_log)
            for idx_model in mcs_list_4:
                model=keras.models.load_model(f'Models/mcs{idx_model}_model.h5')
                pred = model.predict(sinr_log)
                if np.round(pred) == 1:
                    mcs_rate = idx_model
                    break
            mcs_rate = rms.predict(sinr_log[0]);

            print("mcs_index= ", mcs_rate)
            data.pred.mcs[0] = mcs_rate;

            
except KeyboardInterrupt:
    print('Ctrl C')
finally:
    FreeMemory()