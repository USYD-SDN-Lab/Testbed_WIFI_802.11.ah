clear;
clc;

%% config
x_axis = [0.025, 0.05, 0.1, 0.2, 1, 5, 10];
project_prefix = "_dist/";
project_name = ["Vincent_Scenario_WALK_Contention_00_vehicle_0025",
                "Vincent_Scenario_WALK_Contention_00_vehicle_005",
                "Vincent_Scenario_WALK_Contention_00_vehicle_01",
                "Vincent_Scenario_WALK_Contention_00_vehicle_02",
                "Vincent_Scenario_WALK_Contention_00_vehicle_1",
                "Vincent_Scenario_WALK_Contention_00_vehicle_5",
                "Vincent_Scenario_WALK_Contention_00_vehicle_10"];

seeds = ["5", "6", "7"];
RA = ["amrr", "aarf", "minstrel_p10", "minstrel_p25", "minstrel_snn_vincent_p25"];
filename_middle = "/report/throuput_";
filename_suffix = ".csv";
throughputs = zeros(length(RA), length(project_name));

%% update  throughputs 
for raID = 1:length(RA)
    % retrieve data from all projects
    for projectId = 1:length(project_name)
        % for all seeds
        data_all = [];
        for seedId = 1:length(seeds)
            filename = project_prefix + project_name(projectId) + filename_middle + RA(raID) + "_" + seeds(seedId) + filename_suffix;
            if isempty(data_all)
                data_all = readmatrix(filename);
            else
                data_all = data_all + readmatrix(filename);
            end
        end
        data_all = data_all/length(seeds);
        throughputs(raID, projectId) = throughputs(raID, projectId) + data_all(end-3,end);
    end
end

%% plot
plot(x_axis, throughputs(1, 1:end), 'color', "#D3D3D3", 'LineWidth',2, 'MarkerSize', 12);
hold on;
plot(x_axis, throughputs(2, 1:end), 'color', "#EDB120", 'LineWidth',2, 'MarkerSize', 12);
hold on;
plot(x_axis, throughputs(3, 1:end), 'color', "#0072BD", 'LineWidth',2, 'MarkerSize', 12);
hold on;
plot(x_axis, throughputs(4, 1:end), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12);
hold on;
plot(x_axis, throughputs(5, 1:end), 'color', "#D95319", 'LineWidth',2, 'MarkerSize', 12);
hold on;
hold off;
grid on;
ylim([0.14, 0.27])
set(gca,'fontsize',20);
set(gca, 'FontName', 'Times New Roman');
ylabel('Throughput (Mbits/s)'); xlabel('Random Walk Interval (s)');
legend(RA, 'Location','best');

