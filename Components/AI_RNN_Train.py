import pandas
import numpy as np
import sys 
sys.path.append("..");
import os
cur_path = os.getcwd();
os.chdir("./Components");
from Modules.RA_Minstrel_RNN.RA_Minstrel_RNN import RA_Minstrel_RNN;
from Settings import Settings

os.chdir(cur_path);

# load settings
settings = Settings();
# set prefix to store models 
if not os.path.exists(settings.PathRNN()):
    os.makedirs(settings.PathRNN());

# constant parameters
BEACON_SIZE = 71;

# load data
data_frame = pandas.read_csv("Modules/RA_Minstrel_RNN/Examples/TestAveragePower/data/track_yas-wifi-phy_data-beacon.csv", header=None);
data = data_frame.values;
# calculate average power for each beacon
average_power = [];
# record the beacon start parameters
beacon_start = -1;
total_data = 0;
total_weight = 0;
for i in range(0, len(data)):
    if data[i, 0] == BEACON_SIZE:
        # beacon start - update the average power
        if beacon_start != 0:
            if total_weight != 0:
                average_power.append(total_data/total_weight);
        # beacon start - clear the parameters
        beacon_start = data[i, 2];
        total_data = 0;
        total_weight = 0;
    elif beacon_start != -1:
        # beacon data - update the average
        total_data = total_data + data[i, 5]*(data[i, 2] - beacon_start);
        total_weight = total_weight + data[i, 2] - beacon_start;

# transfer the unit from Watts to dBm
average_power = 10*np.log10(average_power) + 30;

# init the model
rmr = RA_Minstrel_RNN(model_prefix =  settings.PathRNN(), model_memory_len=20);
mse_train1, mse_valid1 = rmr.train(0.2, average_power, debug_mse=True);

print("Loss in Train is %f."%mse_train1);
print("Loss in Valid is %f."%mse_valid1);