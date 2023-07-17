clear;
clc;

%% constant parameters
projectname = "NNData";
filename_phy = "_dist/" + projectname +"/report/throuput_amrr_5.csv";
BEACON_SIZE = 71;


%% retrieve the average power for each beacon interval
average_power = [];
beacon_start = -1;
total_data = 0;
total_weight = 0;

for i =1:length(data)
    if data(i, 0) == BEACON_SIZE
        % beacon start - update the average power
        if beacon_start ~= 0
            if total_weight ~= 0
                average_power = [average_power, total_data/total_weight];
            end
        end
        % beacon start - clear the parameters
        beacon_start = data(i, 2);
        total_data = 0;
        total_weight = 0;
    elseif beacon_start ~= -1
        % beacon data - update the average
        total_data = total_data + data(i, 5)*(data(i, 2) - beacon_start);
        total_weight = total_weight + data(i, 2) - beacon_start;
    end
end