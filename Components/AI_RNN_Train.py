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
# rnn settings
model_memory_len = 6;
model_predict_len = 1;
is_dbm = True;

# load data
# set the file names
filename_human = ["rsnn_data_human_5_2", "rsnn_data_human_5_3", "rsnn_data_human_5_4",
                  "rsnn_data_human_6_2", "rsnn_data_human_6_3", "rsnn_data_human_6_4",
                  "rsnn_data_human_7_2", "rsnn_data_human_7_3", "rsnn_data_human_7_4"];
filename_vehicle = ["rsnn_data_vehicle5_2", "rsnn_data_vehicle5_3", "rsnn_data_vehicle5_4",
                    "rsnn_data_vehicle6_2", "rsnn_data_vehicle6_3",
                    "rsnn_data_vehicle7_2", "rsnn_data_vehicle7_3"];
filename_uav = ["rsnn_data_uav5_2", "rsnn_data_uav5_3", "rsnn_data_uav5_4", "rsnn_data_uav5_5",
                "rsnn_data_uav6_2", "rsnn_data_uav6_3", "rsnn_data_uav6_4", "rsnn_data_uav6_5",
                "rsnn_data_uav7_2", "rsnn_data_uav7_3", "rsnn_data_uav7_4"];
filename_human = np.asarray(filename_human);
filename_vehicle = np.asarray(filename_vehicle);
filename_uav = np.asarray(filename_uav);
# merge train filenames
train_filename = np.concatenate((filename_human, filename_vehicle, filename_uav), axis=-1);

# rnn
# build the folder
if not os.path.exists(settings.PathRNN()):
    os.makedirs(settings.PathRNN());
# load rnn
rmr = RA_Minstrel_RNN(model_prefix = settings.PathRNN(), model_memory_len=model_memory_len);
# if untrain, we try to train
if rmr.has_trained():
    print("RNN has already trained");
else:
    # load data
    total_average_power = np.asarray([]);
    for filename in train_filename:
        data_frame = pandas.read_csv("Modules/RA_Minstrel_RNN/Examples/TestAbilityOnAverageData/data/" + filename + "/avernn_0.csv", header=None);
        data = data_frame.values;
        # retrieve average power
        average_power = data[:, 1];
        if is_dbm:
            # remove 0
            average_power = average_power[average_power!=0];
            # to dBm
            average_power = 10*np.log10(average_power) + 30;
        # concatenate
        total_average_power = np.concatenate((total_average_power, average_power), axis=-1);
    # train
    rmr.train(0.2, total_average_power);