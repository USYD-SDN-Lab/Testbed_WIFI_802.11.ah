import sys 
sys.path.append("..");
import os
cur_path = os.getcwd();
os.chdir("./Components");
from Modules.RA_Minstrel_SNN.RA_Minstrel_SNN import RA_Minstrel_SNN;
from Settings import Settings

os.chdir(cur_path);

# load settings
settings = Settings();
# set prefix to store models 
if not os.path.exists(settings.PathSNN()):
    os.makedirs(settings.PathSNN());
# set the train data file
file_train_prefix = "Modules/RA_Minstrel_SNN/Examples/TestAllFunctions/Data/";
file_train_list = ["mcs1.csv",  "mcs2.csv",  "mcs3.csv",  "mcs4.csv",  "mcs5.csv",  "mcs6.csv",  "mcs7.csv",  "mcs8.csv",  "mcs9.csv",
                   "mcs10.csv", "mcs11.csv", "mcs12.csv", "mcs13.csv", "mcs14.csv", "mcs15.csv", "mcs16.csv", "mcs17.csv", "mcs18.csv",
                   "mcs19.csv", "mcs20.csv", "mcs21.csv", "mcs22.csv", "mcs23.csv", "mcs24.csv", "mcs25.csv", "mcs26.csv", "mcs27.csv", "mcs28.csv"];
# set the mcs we need
mcss = [11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28,
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49];
# init the model
rms = RA_Minstrel_SNN(snn_model_prefix = settings.PathSNN());
# train
for mcs_index in range(0, len(mcss)):
    mcs = mcss[mcs_index];
    file_train = file_train_prefix + file_train_list[mcs_index];
    if not rms.is_model_loaded(mcs):
        rms.train(file_train, data_file_mcs_assigend = mcs);
# have trained
print("Trained models: %d/%d"%(rms.get_loaded_models_num(), rms.get_supported_models_num()));