import sys 
sys.path.append("..");
import os
cur_path = os.getcwd();
os.chdir("./Components");
import numpy as np
import torch
from torch import nn
import gc
import math
from py_interface import *
from Modules.RA_Minstrel_SNN.RA_Minstrel_SNN import RA_Minstrel_SNN;
from Modules.NN_TimeLSTM.TimeLSTM_v3 import TimeLSTM_v3 as TimeLSTM
from NNData import memory_pool_size, _SDN_LAB_NNDATA_MEMORY_ID, _SDN_LAB_NNDATA_LEN, _SDN_LAB_MCS_NUM, _SDN_LAB_NNDATA_ILLEGAL_DATA, NNFeature, NNPredicted, NNTarget
from Settings import Settings

os.chdir(cur_path);

# load settings
settings = Settings();
model_memory_len = 20;
is_dbm = True;
# load the SNN
rms = RA_Minstrel_SNN(snn_model_prefix = settings.PathSNN());

lstm_in_feature_num = 1;
lstm_layer_neuron_num = 128;
model = TimeLSTM(TimeLSTM.NN_INIT_TYPE_RANDN, lstm_in_feature_num, lstm_layer_neuron_num, nn_type=TimeLSTM.NN_TYPE_LSTM_TIME1);
model_dnn1 = nn.Linear(lstm_layer_neuron_num, lstm_layer_neuron_num);
model_dnn2 = nn.Linear(lstm_layer_neuron_num, 1);
model.load_state_dict(torch.load("./_build/timelstm/vehicle_5/rnn_lstm_time1_cm/lstm.pkl", map_location=torch.device('cpu')));
model_dnn1.load_state_dict(torch.load("./_build/timelstm/vehicle_5/rnn_lstm_time1_cm/dnn1.pkl", map_location=torch.device('cpu')));
model_dnn2.load_state_dict(torch.load("./_build/timelstm/vehicle_5/rnn_lstm_time1_cm/dnn2.pkl", map_location=torch.device('cpu')));
# to test
model.eval();
model_dnn1.eval();
model_dnn2.eval();


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
            for i in range(_SDN_LAB_NNDATA_LEN-1, -1, -1):
                # now we have the latest data
                if data.feat.rxPower[i] - _SDN_LAB_NNDATA_ILLEGAL_DATA > np.finfo(float).eps:
                    # if we have data, then check wether the length is enough
                    if i >= model_memory_len - 1:
                        is_enough_data = True;
                        data_last_index = i;
                    break;
            
            # no data
            if not is_enough_data:
                print("- there is not enough data, pass");
                for i in range(_SDN_LAB_NNDATA_LEN):
                    print(data.feat.rxPower[i], end=", ");
                print();
                continue;
            else:
                print("- there is enough data at %d"%data_last_index);
                for i in range(_SDN_LAB_NNDATA_LEN):
                    print(data.feat.time[i], end=", ");
                print();
                for i in range(_SDN_LAB_NNDATA_LEN):
                    print(data.feat.rxPower[i], end=", ");
                print();

            # enough data
            data_start_index = data_last_index - model_memory_len + 1;
            data_adjust = [];
            time_adjust = [];
            for i in range(data_start_index, data_last_index + 1):
                data_adjust.append(data.feat.rxPower[i]);
                time_adjust.append(data.feat.time[i]);
            # unit transform
            data_adjust = np.asarray(data_adjust);
            time_adjust = np.asarray(time_adjust);

            if is_dbm:
                data_adjust = 10*np.log10(data_adjust) + 30;
            print(data_adjust);
            print(time_adjust);

            # x_all = data_test_x[idx_file][idx_beacon];
            #         x = torch.from_numpy(x_all[:, :, 0:1]).to(device);
            #         t = torch.from_numpy(x_all[:, :, 1:2]).to(device);
            #         pred_h, pred_cm = model.forward(x, cm=pred_cm, t=t);
            #         dnn_in = torch.squeeze(pred_h, -1);
            #         dnn1_out = model_dnn1_act(model_dnn1(dnn_in));
            #         dnn2_out = model_dnn2(dnn1_out);
            # # predict future RSSI
            # # rssi_future_dbm = rmr.predict(data_adjust);
            
            # # unit transform
            # rssi_future = np.power(10, (rssi_future_dbm - 30)/10);
            # print('  - RSSI=%.4f at %.4f(dBm)'%(rssi_future, rssi_future_dbm), end="");
            # # calculate future SNR
            # snr_future = rssi_future/(No*ch_bws);
            # print(', SNR=%.4f, %.4f, %.4f, '%(snr_future[0], snr_future[1], snr_future[2]), end="");
            # # predict MCS
            # tmp_mcs = 0;
            # tmp_mcs = rms.predict(snr_future[0]);
            # if tmp_mcs < 40:
            #     tmp_mcs = rms.predict(snr_future[1]);
            # elif tmp_mcs < 20:
            #     tmp_mcs = rms.predict(snr_future[2]);
            # data.pred.mcs[0] = tmp_mcs;
            # # print
            # print('MCS=%d'%(data.pred.mcs[0]));
            # # release temporay variables
            # del rssi_future_dbm, rssi_future, snr_future, tmp_mcs;
except KeyboardInterrupt:
    print('Ctrl C')
except Exception as e:
    print('Something wrong')
    print(e)
finally:
    FreeMemory()