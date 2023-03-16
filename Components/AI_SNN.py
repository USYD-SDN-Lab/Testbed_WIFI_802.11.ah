import sys 
sys.path.append("..");
import os
cur_path = os.getcwd();
os.chdir("./Components");
import numpy as np
import math
from py_interface import *
from Modules.RA_Minstrel_SNN.RA_Minstrel_SNN import RA_Minstrel_SNN;
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

# noise settings
BOLTZMANN = 1.3803e-23;                     # Boltzmann constants
tempature = 290;                            # temperature is 290K
noiseFigure = 4.7863;                       # amplify times due to non-idealities of Rx
No = BOLTZMANN*tempature*noiseFigure;       # noise density
ch_bws = [4, 2, 1];                         # the channel bandwidths

# retrieve data
try:
    while True:
        with dl as data:
            print("WAITING FOR DATA");
            if data == None:
                break;
            # retrieve the last SNR
            lastSNR = data.feat.snnLastSnr;
            # predict mcs
            data.pred.snnMcs = rms.predict(lastSNR);
except KeyboardInterrupt:
    print('Ctrl C')
finally:
    FreeMemory()