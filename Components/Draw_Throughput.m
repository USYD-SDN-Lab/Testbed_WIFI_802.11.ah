clear;
clc;

%% project_name
project_name = "rca_test_1sta";

%% end time
start_time = 1;
end_time = 100;

%% load data
% amrr
amrr = [];
isAMRR = true;
% aarf
aarf = [];
isAARF = true;
% minstrel-p10
minstrel_p10 = [];
isMinstrelP10 = true;
% minstrel-p25
minstrel_p25 = [];
isMinstrelP25 = true;
% minstrel-snn-vincent
minstrel_snn_vincent = [];
isMinstrelSNNVincent = true;
% minstrel-snn
minstrel_snn = [];
% minstrel-snn+
misntrel_snn_plus = [];
% minstrel-AI-Dist
misntrel_ai_dist = [];
try
    amrr = readmatrix("_dist/" + project_name +"/report/throuput_amrr.csv");
catch
    isAMRR = false;
end
try
    aarf = readmatrix("_dist/" + project_name +"/report/throuput_aarf.csv");
catch
    isAARF = false;
end
try
    minstrel_p10 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p10.csv");
catch
    isMinstrelP10 = false;
end
try
    minstrel_p25 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p25.csv");
catch
    isMinstrelP25 = false;
end
try
    minstrel_snn_vincent = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_snn_vincent_p25.csv");
catch
    isMinstrelSNNVincent = false;
end

%% set legend
legend_labels = [];
if isAMRR
    legend_labels = [legend_labels, "AMRR"];
end
if isAARF
    legend_labels = [legend_labels, "AARF"];
end
if isMinstrelP10
    legend_labels = [legend_labels, "Minstrel-P10"];
end
if isMinstrelP25
    legend_labels = [legend_labels, "Minstrel-P25"];
end
if isMinstrelSNNVincent
    legend_labels = [legend_labels, "Minstrel-SNN-Vincent"];
end

%% plot - transient throughput
fig1 = figure();
if isAMRR
    plot(amrr(start_time:end_time, 1), amrr(start_time:end_time, 4),'color', "#EDB120",'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isAARF
    plot(aarf(start_time:end_time, 1), aarf(start_time:end_time, 4),'color', "#D3D3D3", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isMinstrelP10
    plot(minstrel_p10(start_time:end_time, 1), minstrel_p10(start_time:end_time, 4), 'color', "#0072BD", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isMinstrelP25
    plot(minstrel_p25(start_time:end_time, 1), minstrel_p25(start_time:end_time, 4), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isMinstrelSNNVincent
    plot(minstrel_snn_vincent(start_time:end_time, 1), minstrel_snn_vincent(start_time:end_time, 4), 'color', "#D95319", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
hold off;
grid on;
set(gca,'fontsize',20);
set(gca, 'FontName', 'Times New Roman');
title('Instantaneous Throughput (per second)');
xlim([start_time, end_time]);
ylabel('Throughput (Mbits/s)'); xlabel('Time(s)');
legend(legend_labels, 'Location','south', 'Orientation','horizontal');

%% plot - total throughput
fig2 = figure();
if isAMRR
    plot(amrr(start_time:end_time, 1), amrr(start_time:end_time, 7),'color', '#EDB120', 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isAARF
    plot(aarf(start_time:end_time, 1), aarf(start_time:end_time, 7),'color', "#D3D3D3", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isMinstrelP10
    plot(minstrel_p10(start_time:end_time, 1), minstrel_p10(start_time:end_time, 7), 'color', "#0072BD", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isMinstrelP25
    plot(minstrel_p25(start_time:end_time, 1), minstrel_p25(start_time:end_time, 7), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if isMinstrelSNNVincent
    plot(minstrel_snn_vincent(start_time:end_time, 1), minstrel_snn_vincent(start_time:end_time, 7), 'color', "#D95319", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
hold off;
grid on;
set(gca,'fontsize',20);
set(gca, 'FontName', 'Times New Roman');
title('Average Throughput');
xlim([start_time, end_time]);
ylabel('Throughput (Mbits/s)'); xlabel('Time(s)');
legend(legend_labels, 'Location','south', 'Orientation','horizontal');