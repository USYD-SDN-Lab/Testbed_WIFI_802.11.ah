import pandas
import numpy as np
import matplotlib.pyplot as plt

# import the data
path_folder ="../../_dist/";
path = "../../_dist/amc_threshold.csv";
data_frame = pandas.read_csv(path)
data = np.array(data_frame.values.astype('float32'));

# retrieve all BER threshold
ber_thresholds = np.unique(data[:, 2]);

# retrive all data rates
data_rates = np.unique(data[:, 1]);

# retrieve the data under BER threshold
for ber_threshold in ber_thresholds:
    data_ber_threshold = data[np.where(data[:, 2]==ber_threshold)];
    data_ber_threshold_sorted_snr = data_ber_threshold[data_ber_threshold[:, 3].argsort()];
    data_ber_threshold_sorted_rssi = data_ber_threshold[data_ber_threshold[:, 4].argsort()];
    
    # gather the information we have to plot
    data_len = len(data_ber_threshold);
    # gather x axis
    x_snr = data_ber_threshold_sorted_snr[:, 3];
    x_rssi = data_ber_threshold_sorted_rssi[:, 4];
    # gather data rate
    y_dr_snr = data_ber_threshold_sorted_snr[:, 1];
    y_dr_rssi = data_ber_threshold_sorted_rssi[:, 1];
    # gather mcs
    y_mcs_snr = data_ber_threshold_sorted_snr[:, 0];
    y_mcs_rssi = data_ber_threshold_sorted_rssi[:, 0];
    # if the new data rate does not increase, we do not use it
    last_dr_snr = 0;
    last_dr_rssi = 0;
    y_mcs_dr_selected_ids = [];
    y_mcs_snr_selected_ids = [];
    y_mcs_rssi_selected_ids = [];
    for cur_id in range(data_len):
        if y_dr_snr[cur_id] >= last_dr_snr:
            last_dr_snr = y_dr_snr[cur_id];
            y_mcs_snr_selected_ids.append(cur_id);
        #
        if y_dr_rssi[cur_id] >= last_dr_rssi:
            last_dr_rssi = y_dr_rssi[cur_id];
            y_mcs_rssi_selected_ids.append(cur_id);
    # to dB
    x_snr = 10*np.log10(x_snr);
    # to dBm
    x_rssi = 10*np.log10(x_rssi*1000);
    # filter data
    x_snr_act = x_snr[y_mcs_snr_selected_ids];
    x_snr_exc = np.delete(x_snr, y_mcs_snr_selected_ids, 0);
    x_rssi_act = x_rssi[y_mcs_rssi_selected_ids];
    x_rssi_exc =  np.delete(x_rssi, y_mcs_rssi_selected_ids, 0);
    y_mcs_snr_act = y_mcs_snr[y_mcs_snr_selected_ids];
    y_mcs_snr_exc = np.delete(y_mcs_snr, y_mcs_snr_selected_ids, 0);
    y_mcs_rssi_act = y_mcs_rssi[y_mcs_rssi_selected_ids];
    y_mcs_rssi_exc = np.delete(y_mcs_rssi, y_mcs_rssi_selected_ids, 0);
    
    y_dr_snr_act = y_dr_snr[y_mcs_snr_selected_ids];
    y_dr_snr_exc = np.delete(y_dr_snr, y_mcs_snr_selected_ids, 0);
    y_dr_rssi_act = y_dr_rssi[y_mcs_rssi_selected_ids];
    y_dr_rssi_exc = np.delete(y_dr_rssi, y_mcs_rssi_selected_ids, 0);
    # reset MCS 100 to 0
    y_mcs_snr_act[y_mcs_snr_act==110] = 0;
    y_mcs_rssi_act[y_mcs_rssi_act==110] = 0;
    
   
    
    # plot - SNR
    plt.figure(figsize=(15, 6), dpi=200)
    plt.plot(x_snr_act, y_mcs_snr_act, '-o');
    for data_id in range(len(y_mcs_snr_act)):
        plt.text(x_snr_act[data_id], y_mcs_snr_act[data_id], str(int(y_mcs_snr_act[data_id])) + ':' + str(int(y_dr_snr_act[data_id])), va='top');
    # ignore points
    plt.scatter(x_snr_exc, y_mcs_snr_exc);
    for data_id in range(len(y_mcs_snr_exc)):
        plt.text(x_snr_exc[data_id], y_mcs_snr_exc[data_id], str(int(y_mcs_snr_exc[data_id])) + ':' + str(int(y_dr_snr_exc[data_id])), va='bottom');
    plt.xlabel('SNR(dBm)');
    plt.ylabel('MCS')
    plt.title('MCS vs SNR (BER=' + str(ber_threshold) + ')');
    plt.grid();
    plt.savefig(path_folder + 'MCS vs SNR (BER=' + str(ber_threshold) + ').jpg');
    
    # plot - RSSI
    plt.figure(figsize=(15, 6), dpi=200)
    plt.plot(x_rssi_act, y_mcs_rssi_act, '-o');
    for data_id in range(len(y_mcs_rssi_act)):
        plt.text(x_rssi_act[data_id], y_mcs_rssi_act[data_id], str(int(y_mcs_rssi_act[data_id])) + ':' + str(int(y_dr_rssi_act[data_id])), va='top');
    # ignore points
    plt.scatter(x_rssi_exc, y_mcs_rssi_exc);
    for data_id in range(len(y_mcs_rssi_exc)):
        plt.text(x_rssi_exc[data_id], y_mcs_rssi_exc[data_id], str(int(y_mcs_rssi_exc[data_id])) + ':' + str(int(y_dr_rssi_exc[data_id])), va='top');
    plt.xlabel('RSSI(dBm)');
    plt.ylabel('MCS')
    plt.title('MCS vs RSSI (BER=' + str(ber_threshold) + ')');
    plt.grid();
    plt.savefig(path_folder + 'MCS vs RSSI (BER=' + str(ber_threshold) + ').jpg');
    
    
    pass