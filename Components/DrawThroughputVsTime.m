% draw the throughput vs time
% <INPUT>
% @foldername:      the folername
% @seeds:           the seed range (a number vector)
% @jumptime:        the time we jump
% @duration:        the durition we take records
% @ColNumTime:      the column number for time
% @ColNumTransient: the column number for transient power
% @ColNumAverage:   the column number for average power
% @PlotTransient:   plot the transient power (default: true)
% @PlotAverage:     plot the average power (default: false)
function DrawThroughputVsTime(foldername, varargin)
    clc;
    
    % default values
    seeds = [5 6 7];
    jumptime = 9;       % we start at time point (jumptime + 1) is 10
    duration = 100;
    % column meanings
    col_num_time        = 1;
    col_num_transient   = 4;
    col_num_average     = 7;
    % plot
    plot_transient = true;
    plot_average= false;
    % filenames
    path_prefix     = "_dist/" + foldername + "/report/";
    filesuffix      = ".csv";
    
    % register all optional inputs
    inPar = inputParser;
    % register and set default
    addParameter(inPar,'seeds', seeds, @(x) isnumeric(x)&&isvector(x));                         % seeds
    addParameter(inPar,'jumptime', jumptime, @(x) isnumeric(x)&&isscalar(x));                   % jumptime
    addParameter(inPar,'duration', duration, @(x) isnumeric(x)&&isscalar(x));                   % duration
    addParameter(inPar,'ColNumTime', col_num_time, @(x) isnumeric(x)&&isscalar(x));             % col_num_time
    addParameter(inPar,'ColNumTransient', col_num_transient, @(x) isnumeric(x)&&isscalar(x));   % col_num_transient
    addParameter(inPar,'ColNumAverage', col_num_average, @(x) isnumeric(x)&&isscalar(x));       % col_num_average
    addParameter(inPar,'PlotTransient', plot_transient, @islogical);                            % plot_transient
    addParameter(inPar,'PlotAverage', plot_average, @islogical);                                % plot_average
    % Allow unmatched cases
    inPar.KeepUnmatched = true;
    % Allow capital or small characters
    inPar.CaseSensitive = false;
    % parse
    parse(inPar, varargin{:});
    % take values
    seeds = inPar.Results.seeds;
    col_num_time = inPar.Results.ColNumTime;
    col_num_transient = inPar.Results.ColNumTransient;
    col_num_average = inPar.Results.ColNumAverage;
    plot_transient = inPar.Results.PlotTransient;
    plot_average = inPar.Results.PlotAverage;
    
    % RA data storage
    ra_labels = {"AARF", "AMRR", "Minstrel(10%)", "Minstrel(25%)", "SNN", "RNN"};
    ra_filenames = ["throuput_aarf_",
                    "throuput_amrr_",
                    "throuput_minstrel_p10_",
                    "throuput_minstrel_p25_",
                    "throuput_minstrel_snn_vincent_p25_",
                    "throuput_minstrel_snn_plus_p25_"];
    ra_time = {[], [], [], [], [], []};
    ra_throughput_transient = {[], [], [], [], [], []};
    ra_throughput_average = {[], [], [], [], [], []};
    ra_data_len = [0, 0, 0, 0, 0, 0];
    ra_plot_colors = ["#EDB120", "#D3D3D3", "#0072BD", "#4DBEEE", "#D95319", "#7E2F8E"];
    
    % read data
    for idx_ra = 1:length(ra_filenames)
        curfileprefix = path_prefix + ra_filenames(idx_ra);
        for seed = seeds
            try
                curfile = curfileprefix + seed + filesuffix;
                curdata = readmatrix(curfile);
                % tmp data
                tmp_time = curdata(:, col_num_time);
                tmp_transient = curdata(:, col_num_transient);
                tmp_average = curdata(:, col_num_average);
                % get data range
                data_range = (tmp_time > jumptime & tmp_time <= jumptime + duration);
                % assign time
                ra_time{idx_ra} = tmp_time(data_range);
                % assign transient
                if isempty(ra_throughput_transient{idx_ra})
                    ra_throughput_transient{idx_ra} = tmp_transient(data_range);
                else
                    ra_throughput_transient{idx_ra} = ra_throughput_transient{idx_ra} + tmp_transient(data_range);
                end
                % assign average
                if isempty(ra_throughput_average{idx_ra})
                    ra_throughput_average{idx_ra} = tmp_average(data_range);
                else
                    ra_throughput_average{idx_ra} = ra_throughput_average{idx_ra} + tmp_average(data_range);
                end
                % we have added one set of data
                ra_data_len(idx_ra) = ra_data_len(idx_ra) + 1;
            catch
                warning("File not exist: " + curfile);
            end
        end
    end
    
    % calculate average
    for idx_ra = 1:length(ra_filenames)
        if ra_data_len(idx_ra) > 0
            ra_throughput_transient{idx_ra} = ra_throughput_transient{idx_ra}/ra_data_len(idx_ra);
            ra_throughput_average{idx_ra} = ra_throughput_average{idx_ra}/ra_data_len(idx_ra);
        end
    end
    
    % plot
    % plot labels
    legend_labels = [];
    for idx_ra = 1:length(ra_filenames)
        if ra_data_len(idx_ra) > 0
            legend_labels = [legend_labels ra_labels{idx_ra}];
        end
    end
    % plot - transient
    if plot_transient
        figure();
        for idx_ra = 1:length(ra_filenames)
            if ra_data_len(idx_ra) > 0
                plot(ra_time{idx_ra}, ra_throughput_transient{idx_ra}, 'color', ra_plot_colors(idx_ra), 'LineWidth',2, 'MarkerSize', 12);
                hold on;
            end
        end
        hold off;
        grid on;
        xlim([jumptime+1, jumptime+duration]);
        ylabel('Throughput (Mbits/s)');
        xlabel('Time(s)');
        legend(legend_labels, 'Location','best');
    end
    % plot - average
    if plot_average
        figure();
        for idx_ra = 1:length(ra_filenames)
            if ra_data_len(idx_ra) > 0
                plot(ra_time{idx_ra}, ra_throughput_average{idx_ra}, 'color', ra_plot_colors(idx_ra), 'LineWidth',2, 'MarkerSize', 12);
                hold on;
            end
        end
        hold off;
        grid on;
        xlim([jumptime+1, jumptime+duration]);
        ylabel('Throughput (Mbits/s)');
        xlabel('Time(s)');
        legend(legend_labels, 'Location','best');
    end
end

