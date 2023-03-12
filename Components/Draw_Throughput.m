clear;
clc;

% load data
amrr = readmatrix('_dist/rca/report/throuput_amrr.csv');
aarf = readmatrix('_dist/rca/report/throuput_aarf.csv');
minstrel = readmatrix('_dist/rca/report/throuput_minstrel.csv');

% plot - transient throughput
fig1 = figure();
plot(amrr(:, 1), amrr(:, 4),'black','LineWidth',2, 'MarkerSize', 12);
hold on;
plot(aarf(:, 1), aarf(:, 4),'color', "#EDB120", 'LineWidth',2, 'MarkerSize', 12);
hold on;
plot(minstrel(:, 1), minstrel(:, 4), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12);
grid on;
set(gca,'fontsize',20);
set(gca, 'FontName', 'Times New Roman');
title('Instantaneous Throughput (per second)');
ylabel('Throughput (Mbits/s)'); xlabel('Time(s)');
legend('AMRR', 'AARF', 'Minstrel', 'Location','south', 'Orientation','horizontal');

% plot - total throughput
fig2 = figure();
plot(amrr(:, 1), amrr(:, 7),'black','LineWidth',2, 'MarkerSize', 12);
hold on;
plot(aarf(:, 1), aarf(:, 7),'color', "#EDB120", 'LineWidth',2, 'MarkerSize', 12);
hold on;
plot(minstrel(:, 1), minstrel(:, 7), 'color', "#4DBEEE", 'LineWidth',2, 'MarkerSize', 12);
grid on;
set(gca,'fontsize',20);
set(gca, 'FontName', 'Times New Roman');
title('Average Throughput');
ylabel('Throughput (Mbits/s)'); xlabel('Time(s)');
legend('AMRR', 'AARF', 'Minstrel', 'Location','south', 'Orientation','horizontal');