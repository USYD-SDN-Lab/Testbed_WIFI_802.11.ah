clear;
clc;

%% project_name
project_name = "Vincent_Scenario_Static_Contention_04_std";
%project_name2 = "Vincent_Scenario_RWM_Contention_04_vehicle";

%% end time
start_time = 10;
end_time = 110;
time_steps = 0:(end_time - start_time);

%% load data
legend_labels = []; %legend
legend_labels_fixed = [];
legend_RWM = [];

try
    amrr5 = readmatrix("_dist/" + project_name +"/report/throuput_amrr_5.csv");
    amrr6 = readmatrix("_dist/" + project_name +"/report/throuput_amrr_6.csv");
    amrr7 = readmatrix("_dist/" + project_name +"/report/throuput_amrr_7.csv");
    amrr = (amrr5 + amrr6 + amrr7)/3;
    legend_labels = [legend_labels, "AMRR(fixed)"];
    legend_labels_fixed = [legend_labels_fixed , "AMRR(fixed)"];
    amrr5 = readmatrix("_dist/" + project_name2 +"/report/throuput_amrr_5.csv");
    amrr6 = readmatrix("_dist/" + project_name2 +"/report/throuput_amrr_6.csv");
    amrr7 = readmatrix("_dist/" + project_name2 +"/report/throuput_amrr_7.csv");
    amrr_RWM = (amrr5 + amrr6 + amrr7)/3;
    legend_labels = [legend_labels, "AMRR(RWM)"];
    legend_RWM = [legend_RWM, "AMRR(RWM)"];
    clear amrr5 amrr6 amrr7;
catch
    isAMRR = false;
end
try
    aarf5 = readmatrix("_dist/" + project_name +"/report/throuput_aarf_5.csv");
    aarf6 = readmatrix("_dist/" + project_name +"/report/throuput_aarf_6.csv");
    aarf7 = readmatrix("_dist/" + project_name +"/report/throuput_aarf_7.csv");
    aarf = (aarf5 + aarf6 + aarf7)/3;
    legend_labels = [legend_labels, "AARF(fixed)"];
    legend_labels_fixed = [legend_labels_fixed, "AARF(fixed)"];
    aarf5 = readmatrix("_dist/" + project_name2 +"/report/throuput_aarf_5.csv");
    aarf6 = readmatrix("_dist/" + project_name2 +"/report/throuput_aarf_6.csv");
    aarf7 = readmatrix("_dist/" + project_name2 +"/report/throuput_aarf_7.csv");
    aarf_RWM = (aarf5 + aarf6 + aarf7)/3;
    legend_labels = [legend_labels, "AARF(RWM)"];
    legend_RWM = [legend_RWM, "AARF(RWM)"];
    clear aarf5 aarf6 aarf7;
catch
    isAARF = false;
end
try
    minstrel_p105 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p10_5.csv");
    minstrel_p106 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p10_6.csv");
    minstrel_p107 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p10_7.csv");
    minstrel_p10 = (minstrel_p105 + minstrel_p106 + minstrel_p107)/3;
    legend_labels = [legend_labels, "Minstrel-P10(fixed)"];
    legend_labels_fixed = [legend_labels_fixed, "Minstrel-P10(fixed)"];
    minstrel_p105 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_p10_5.csv");
    minstrel_p106 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_p10_6.csv");
    minstrel_p107 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_p10_7.csv");
    minstrel_p10_RWM = (minstrel_p105 + minstrel_p106 + minstrel_p107)/3;
    legend_labels = [legend_labels, "Minstrel-P10(RWM)"];
    legend_RWM = [legend_RWM, "Minstrel-P10(RWM)"];
    clear minstrel_p105 minstrel_p106 minstrel_p107;
catch
    isMinstrelP10 = false;
end
try
    minstrel_p255 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p25_5.csv");
    minstrel_p256 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p25_6.csv");
    minstrel_p257 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_p25_7.csv");
    minstrel_p25 = (minstrel_p255 + minstrel_p256 + minstrel_p257)/3;
    legend_labels = [legend_labels, "Minstrel-P25(fixed)"];
    legend_labels_fixed = [legend_labels_fixed, "Minstrel-P25(fixed)"];
    minstrel_p255 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_p25_5.csv");
    minstrel_p256 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_p25_6.csv");
    minstrel_p257 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_p25_7.csv");
    minstrel_p25_RWM = (minstrel_p255 + minstrel_p256 + minstrel_p257)/3;
    legend_labels = [legend_labels, "Minstrel-P25(RWM)"];
    legend_RWM = [legend_RWM, "Minstrel-P25(RWM)"];
    clear minstrel_p255 minstrel_p256 minstrel_p257;
catch
    isMinstrelP25 = false;
end
try
    minstrel_snn_vincent5 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_snn_vincent_p25_5.csv");
    minstrel_snn_vincent6 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_snn_vincent_p25_6.csv");
    %minstrel_snn_vincent7 = readmatrix("_dist/" + project_name +"/report/throuput_minstrel_snn_vincent_p25_7.csv");
    minstrel_snn_vincent = (minstrel_snn_vincent5 + minstrel_snn_vincent6)/2;
    legend_labels = [legend_labels, "Minstrel-SNN(fixed)"];
    legend_labels_fixed = [legend_labels_fixed, "Minstrel-SNN(fixed)"];
    minstrel_snn_vincent5 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_snn_vincent_p25_5.csv");
    minstrel_snn_vincent6 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_snn_vincent_p25_6.csv");
    minstrel_snn_vincent7 = readmatrix("_dist/" + project_name2 +"/report/throuput_minstrel_snn_vincent_p25_7.csv");
    minstrel_snn_vincent_RWM = (minstrel_snn_vincent5 + minstrel_snn_vincent6 + minstrel_snn_vincent7)/3;
    legend_labels = [legend_labels, "Minstrel-SNN(RWM)"];
    legend_RWM = [legend_RWM, "Minstrel-SNN(RWM)"];
    clear minstrel_snn_vincent5 minstrel_snn_vincent6 minstrel_snn_vincent7;
    
catch
    isMinstrelSNNVincent = false;
end

%% plot - transient throughput
fig1 = figure();
f_fixed = [];
f_RWM = [];
%AMRR
if exist('amrr')
    f_amrr = plot(time_steps, amrr(start_time:end_time, 4),'color', "#D3D3D3",'LineWidth',2, 'MarkerSize', 12);
    f_fixed = [f_fixed, f_amrr];
    hold on;
end
if exist('amrr_RWM')
    f_amrr_RWM = plot(time_steps, amrr_RWM(start_time:end_time, 4),'color', "#D3D3D3",'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    f_RWM = [f_RWM, f_amrr_RWM];
    hold on;
end
% AARF
if exist('aarf')
    f_aarf = plot(time_steps, aarf(start_time:end_time, 4),'color', "#EDB120", 'LineWidth',2, 'MarkerSize', 12);
    f_fixed = [f_fixed, f_aarf];
    hold on;
end
if exist('aarf_RWM')
    f_aarf_RWM = plot(time_steps, aarf_RWM(start_time:end_time, 4),'color', "#EDB120", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    f_RWM = [f_RWM, f_aarf_RWM];
    hold on;
end
if exist('minstrel_p10')
    f_minstrel_p10 = plot(time_steps, minstrel_p10(start_time:end_time, 4), 'color', "#0072BD", 'LineWidth',2, 'MarkerSize', 12);
    f_fixed = [f_fixed, f_minstrel_p10];
    hold on;
end
if exist('minstrel_p10_RWM')
    f_minstrel_p10_RWM = plot(time_steps, minstrel_p10_RWM(start_time:end_time, 4), 'color', "#0072BD", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    f_RWM = [f_RWM, f_minstrel_p10_RWM];
    hold on;
end
% Minstrel P25
if exist('minstrel_p25')
    f_minstrel_p25 = plot(time_steps, minstrel_p25(start_time:end_time, 4), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12);
    f_fixed = [f_fixed, f_minstrel_p25];
    hold on;
end
if exist('minstrel_p25_RWM')
    f_minstrel_p25_RWM = plot(time_steps, minstrel_p25_RWM(start_time:end_time, 4), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    f_RWM = [f_RWM, f_minstrel_p25_RWM];
    hold on;
end
if exist('minstrel_snn_vincent')
    f_minstrel_snn_vincent = plot(time_steps, minstrel_snn_vincent(start_time:end_time, 4), 'color', "#D95319", 'LineWidth',2, 'MarkerSize', 12);
    f_fixed = [f_fixed, f_minstrel_snn_vincent];
    hold on;
end
if exist('minstrel_snn_vincent_RWM')
    f_minstrel_snn_vincent_RWM = plot(time_steps, minstrel_snn_vincent_RWM(start_time:end_time, 4), 'color', "#D95319", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    f_RWM = [f_RWM, f_minstrel_snn_vincent_RWM];
    hold on;
end
hold off

hold off;
grid on;
set(gca,'fontsize',20);
set(gca, 'FontName', 'Times New Roman');
title('Instantaneous Throughput (per second)');
ylabel('Throughput (Mbits/s)'); xlabel('Time(s)');
legend(legend_labels, 'Location','south', 'Orientation','horizontal');
%legend(legend_labels, 'Location','south', 'Orientation','horizontal', 'NumColumns', 2);
%legend(f_fixed, legend_labels_fixed, 'Location','south', 'Orientation','horizontal');
%legend(f_RWM, legend_RWM, 'Location','south', 'Orientation','horizontal');


%% plot - total throughput
fig2 = figure();
if exist('amrr')
    plot(time_steps, amrr(start_time:end_time, 7),'color', "#D3D3D3", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if exist('amrr_RWM')
    plot(time_steps, amrr_RWM(start_time:end_time, 7),'color', "#D3D3D3", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    hold on;
end
if exist('aarf')
    plot(time_steps, aarf(start_time:end_time, 7),'color', '#EDB120', 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if exist('aarf_RWM')
    plot(time_steps, aarf_RWM(start_time:end_time, 7),'color', '#EDB120', 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    hold on;
end
if exist('minstrel_p10')
    plot(time_steps, minstrel_p10(start_time:end_time, 7), 'color', "#0072BD", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if exist('minstrel_p10_RWM')
    plot(time_steps, minstrel_p10_RWM(start_time:end_time, 7), 'color', "#0072BD", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    hold on;
end
if exist('minstrel_p25')
    plot(time_steps, minstrel_p25(start_time:end_time, 7), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if exist('minstrel_p25_RWM')
    plot(time_steps, minstrel_p25_RWM(start_time:end_time, 7), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    hold on;
end
if exist('minstrel_snn_vincent')
    plot(time_steps, minstrel_snn_vincent(start_time:end_time, 7), 'color', "#D95319", 'LineWidth',2, 'MarkerSize', 12);
    hold on;
end
if exist('minstrel_snn_vincent_RWM')
    plot(time_steps, minstrel_snn_vincent_RWM(start_time:end_time, 7), 'color', "#D95319", 'LineWidth',2, 'MarkerSize', 12, 'LineStyle', "--");
    hold on;
end
hold off;
grid on;
set(gca,'fontsize',20);
set(gca, 'FontName', 'Times New Roman');
title('Average Throughput');
xlim([start_time, end_time]);
ylabel('Throughput (Mbits/s)'); xlabel('Time(s)');
legend(legend_labels, 'Location','northeastoutside');