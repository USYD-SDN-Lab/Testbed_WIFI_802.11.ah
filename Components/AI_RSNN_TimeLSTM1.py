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
model_memory_len = 16;
is_dbm = True;
# load the SNN
rms = RA_Minstrel_SNN(snn_model_prefix = settings.PathSNN());

lstm_in_feature_num = 1;
lstm_layer_neuron_num = 128;
cell_memory = [None]*500;

model = TimeLSTM(TimeLSTM.NN_INIT_TYPE_RANDN, lstm_in_feature_num, lstm_layer_neuron_num, nn_type=TimeLSTM.NN_TYPE_LSTM_TIME1);
model_dnn1 = nn.Linear(lstm_layer_neuron_num, lstm_layer_neuron_num);
model_dnn2 = nn.Linear(lstm_layer_neuron_num, 1);
model.load_state_dict(torch.load("./_build/timelstm/vehicle_5/rnn_lstm_time1_cm/lstm.pkl", map_location=torch.device('cpu')));
model_dnn1.load_state_dict(torch.load("./_build/timelstm/vehicle_5/rnn_lstm_time1_cm/dnn1.pkl", map_location=torch.device('cpu')));
model_dnn2.load_state_dict(torch.load("./_build/timelstm/vehicle_5/rnn_lstm_time1_cm/dnn2.pkl", map_location=torch.device('cpu')));
model = model.to(torch.float64);
model_dnn1 = model_dnn1.to(torch.float64);
model_dnn2 = model_dnn2.to(torch.float64);
# to test
model.eval();
model_dnn1.eval();
model_dnn2.eval();

def get_throughput(mcs_idx):
    if mcs_idx == 110:
        return 150;
    if mcs_idx == 10:
        return 300;
    if mcs_idx == 11:
        return 600;
    if mcs_idx == 12:
        return 900;
    if mcs_idx == 13:
        return 1200;
    if mcs_idx == 14:
        return 1800;
    if mcs_idx == 15:
        return 2400;
    if mcs_idx == 16:
        return 2700;
    if mcs_idx == 17:
        return 3000;
    if mcs_idx == 18:
        return 3600;
    if mcs_idx == 19:
        return 4000;
    if mcs_idx == 20:
        return 650;
    if mcs_idx == 21:
        return 1300;
    if mcs_idx == 22:
        return 1950;
    if mcs_idx == 23:
        return 2600;
    if mcs_idx == 24:
        return 3900;
    if mcs_idx == 25:
        return 5200;
    if mcs_idx == 26:
        return 5850;
    if mcs_idx == 27:
        return 6500;
    if mcs_idx == 28:
        return 7800;
    if mcs_idx == 40:
        return 1350;
    if mcs_idx == 41:
        return 2700;
    if mcs_idx == 42:
        return 4050;
    if mcs_idx == 43:
        return 5400;
    if mcs_idx == 44:
        return 8100;
    if mcs_idx == 45:
        return 10800;
    if mcs_idx == 46:
        return 12150;
    if mcs_idx == 47:
        return 13500;
    if mcs_idx == 48:
        return 16200;
    if mcs_idx == 49:
        return 18000;
    return 0;

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
            staid = data.feat.staid;
            print("STA %d"%staid);
            # if the input is illegal, we do nothing
            if data.feat.rxPower[0] == _SDN_LAB_NNDATA_ILLEGAL_DATA:
                print("  - No feature, pass");
                pass


            # check whether we have data
            is_enough_data = False;
            data_last_index = 0;
            for i in range(_SDN_LAB_NNDATA_LEN-1, -1, -1):
                # now we have the latest data
                if data.feat.rxPower[i] > 0 and data.feat.bandwidth[i] != 0:
                    # check wether the length is enough
                    if i >= model_memory_len - 1:
                        is_enough_data = True;
                    # we have the last available data
                    data_last_index = i;
                    break;
            
            # no data - use SNN
            if not is_enough_data:
                print("  - no enough data, rssi=%.4f, bandwidth=%d"%(data.feat.rxPower[data_last_index], data.feat.bandwidth[data_last_index]));
                lastRxPower = data.feat.rxPower[data_last_index];
                lastBandwidth = data.feat.bandwidth[data_last_index];
                lastSNR = lastRxPower/(No*lastBandwidth);
                # predict mcs
                data.pred.mcs[0] = rms.predict(lastSNR);
                print("  - SNN(not enough data): SNR=%.4f, MCS=%d"%(lastSNR, data.pred.mcs[0]));
                for i in range(_SDN_LAB_NNDATA_LEN):
                    print(data.feat.rxPower[i], end=", ");
                print();
                del lastRxPower, lastBandwidth, lastSNR;
                continue;
            else:
                print("  - there is enough data at %d"%data_last_index);
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
            # reshape
            data_adjust = np.expand_dims(data_adjust, axis=(0, -1));
            time_adjust = np.expand_dims(time_adjust, axis=(0, -1));

            if is_dbm:
                data_adjust = 10*np.log10(data_adjust) + 30;
            # predict future RSSI
            x = torch.from_numpy(data_adjust);
            t = torch.from_numpy(time_adjust);
            cm_cur = cell_memory[staid];
            pred_h, pred_cm = model.forward(x, cm=cm_cur, t=t);
            dnn_in = torch.squeeze(pred_h, -1);
            dnn1_out = torch.tanh(model_dnn1(dnn_in));
            dnn2_out = model_dnn2(dnn1_out);
            
            pred_cm = pred_cm.detach();
            cell_memory[staid] = pred_cm;
            rssi_future_dbm = dnn2_out.detach().numpy();

            rssi_future_dbm = np.squeeze(rssi_future_dbm);

            del data_adjust, time_adjust, x, t, pred_h, pred_cm, dnn_in, dnn1_out, dnn2_out;
            
            # unit transform
            rssi_future = np.power(10, (rssi_future_dbm - 30)/10);
            print('  - RSSI=%.4f at %.4f(dBm)'%(rssi_future, rssi_future_dbm), end="");
            # calculate future SNR
            snr_future = rssi_future/(No*ch_bws);
            print(', SNR=%.4f, %.4f, %.4f, '%(snr_future[0], snr_future[1], snr_future[2]), end="");
            # predict MCS
            tmp_mcs = 0;
            tmp_mcs0 = rms.predict(snr_future[0]);
            tmp_mcs1 = rms.predict(snr_future[1]);
            tmp_mcs2 = rms.predict(snr_future[2]);
            if tmp_mcs0 < 40:
                tmp_mcs0 = 0;
            if tmp_mcs1 < 20 or tmp_mcs1 >= 40:
                tmp_mcs1 = 0;
            if tmp_mcs2 >= 20:
                tmp_mcs2 = 0;
            tmp_mcs0_throughput = get_throughput(tmp_mcs0);
            tmp_mcs1_throughput = get_throughput(tmp_mcs1);
            tmp_mcs2_throughput = get_throughput(tmp_mcs2);
            max_throughput = max([tmp_mcs0_throughput, tmp_mcs1_throughput, tmp_mcs2_throughput]);
            tmp_mcs = 10;
            if tmp_mcs0_throughput == max_throughput:
                tmp_mcs = tmp_mcs0;
            elif tmp_mcs1_throughput == max_throughput:
                tmp_mcs = tmp_mcs1;
            elif tmp_mcs2_throughput == max_throughput:
                tmp_mcs = tmp_mcs2;

            data.pred.mcs[0] = tmp_mcs;
            # print
            print('MCS=%d'%(data.pred.mcs[0]));
            # release temporay variables
            del rssi_future_dbm, rssi_future, snr_future, tmp_mcs;
except KeyboardInterrupt:
    print('Ctrl C')
except Exception as e:
    print('Something wrong')
    print(e)
finally:
    FreeMemory()