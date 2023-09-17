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
    data_ber_threshold_sorted_dr = data_ber_threshold[data_ber_threshold[:, 1].argsort()];
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
    # filter data
    x_snr = x_snr[y_mcs_snr_selected_ids];
    x_rssi = x_rssi[y_mcs_rssi_selected_ids];
    y_mcs_snr = y_mcs_snr[y_mcs_snr_selected_ids];
    y_mcs_rssi = y_mcs_rssi[y_mcs_rssi_selected_ids];
    # reset MCS 100 to 0
    y_mcs_snr[y_mcs_snr==110] = 0;
    y_mcs_rssi[y_mcs_rssi==110] = 0;
    # to dB
    x_snr = 10*np.log10(x_snr);
    # to dBm
    x_rssi = 10*np.log10(x_rssi*1000);
    
    # plot - SNR
    plt.figure(figsize=(15, 6), dpi=200)
    plt.plot(x_snr, y_mcs_snr, '-o');
    for data_id in range(len(y_mcs_snr)):
        plt.text(x_snr[data_id], y_mcs_snr[data_id], str(int(y_mcs_snr[data_id])), va='top');
    plt.xlabel('SNR(dBm)');
    plt.ylabel('MCS')
    plt.title('MCS vs SNR (BER=' + str(ber_threshold) + ')');
    plt.grid();
    plt.savefig(path_folder + 'MCS vs SNR (BER=' + str(ber_threshold) + ')');
    
    # plot - RSSI
    plt.figure(figsize=(15, 6), dpi=200)
    plt.plot(x_rssi, y_mcs_rssi, '-o');
    for data_id in range(len(y_mcs_rssi)):
        plt.text(x_rssi[data_id], y_mcs_rssi[data_id], str(int(y_mcs_rssi[data_id])), va='top');
    plt.xlabel('RSSI(dBm)');
    plt.ylabel('MCS')
    plt.title('MCS vs RSSI (BER=' + str(ber_threshold) + ')');
    plt.grid();
    plt.savefig(path_folder + 'MCS vs RSSI (BER=' + str(ber_threshold) + ')');
    
    
    pass