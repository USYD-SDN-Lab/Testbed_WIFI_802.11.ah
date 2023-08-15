% draw the throughput vs holdtime (using the average throughput)
% <INPUT>
% @projectnameprefix:       the project name prefix
% @holdtime:                the holdtime range(a number vector)
% @seeds:                   the seed range (a number vector)
% @endtime:                 the endtime point (a scalar)
% @ColNumTime:              the column number for time
% @ColNumAverage:           the column number for average power
function DrawThroughputVsHoldTime(projectnameprefix, varargin)
    clc;
    
    % default values
    holdtime = [1 2 3 4 5];
    seeds = [5 6 7];
    endtime = 110;
    % column meanings
    col_num_time        = 1;
    col_num_average     = 7;
    % path & file
    path_prefix                 = "_dist/" + projectnameprefix;             % uniform prefix
    path_report                 = "/report/";                               % report folder
    filesuffix                  = ".csv";
    
    % register all optional inputs
    inPar = inputParser;
    % register and set default
    addParameter(inPar,'holdtime', holdtime, @(x) isnumeric(x)&&isvector(x));                   % holdtime
    addParameter(inPar,'seeds', seeds, @(x) isnumeric(x)&&isvector(x));                         % seeds
    addParameter(inPar,'EndTime', endtime, @(x) isnumeric(x)&&isscalar(x));                     % endtime
    addParameter(inPar,'ColNumTime', col_num_time, @(x) isnumeric(x)&&isscalar(x));             % col_num_time
    addParameter(inPar,'ColNumAverage', col_num_average, @(x) isnumeric(x)&&isscalar(x));       % col_num_average
    % Allow unmatched cases
    inPar.KeepUnmatched = true;
    % Allow capital or small characters
    inPar.CaseSensitive = false;
    % parse
    parse(inPar, varargin{:});
    % take values
    holdtime                = inPar.Results.holdtime;
    seeds                   = inPar.Results.seeds;
    endtime                 = inPar.Results.EndTime;
    col_num_time            = inPar.Results.ColNumTime;
    col_num_average         = inPar.Results.ColNumAverage;
    if length(holdtime) <= 0
        error("[holdtime] is empty.");
    else
        for idx_holdtime = 1:length(holdtime)
            if holdtime(idx_holdtime) <= 0
                error("holdtime(" + idx_holdtime + ") is not positive.");
            end
        end
    end
    
    % RA data storage
    ra_labels = {"AARF", "AMRR", "Minstrel(10%)", "Minstrel(25%)", "SNN", "RNN"};
    ra_filenames = ["throuput_aarf_",
                    "throuput_amrr_",
                    "throuput_minstrel_p10_",
                    "throuput_minstrel_p25_",
                    "throuput_minstrel_snn_vincent_p25_",
                    "throuput_minstrel_snn_plus_p25_"];
    ra_throughput_average = zeros(length(holdtime), length(ra_filenames));
    ra_plot_colors = ["#EDB120", "#D3D3D3", "#0072BD", "#4DBEEE", "#D95319", "#7E2F8E"];
    
    % read data
    for idx_holdtime = 1:length(holdtime)
        curfolder = path_prefix + holdtime(idx_holdtime) + path_report;
        % for each algorithm
        for idx_ra = 1:length(ra_filenames)
            curfileprefix = curfolder + ra_filenames(idx_ra);
            % accumulate data
            tmp_data_len = 0;
            for seed = seeds
                curfile = curfileprefix + seed + filesuffix;
                try
                    % read data
                    curdata = readmatrix(curfile);
                    tmp_time = curdata(:, col_num_time);
                    tmp_average = curdata(:, col_num_average);
                    endpoint = (tmp_time == endtime);
                    ra_throughput_average(idx_holdtime, idx_ra) = ra_throughput_average(idx_holdtime, idx_ra) + tmp_average(endpoint);
                    % we have added one piece of data
                    tmp_data_len = tmp_data_len + 1;
                catch e
                    switch e.identifier
                        case "MATLAB:textio:textio:FileNotFound"
                            warning("File not exist: " + curfile);
                        case "MATLAB:subsassigndimmismatch"
                            warning("[endtime=" + endtime + "] does not has any data");
                        otherwise
                            rethrow(e);
                    end                    
                end
            end
            % average data
            if tmp_data_len > 0
                ra_throughput_average(idx_holdtime, idx_ra) = ra_throughput_average(idx_holdtime, idx_ra)/tmp_data_len;
            end
        end
    end
    
    % plot
    % plot - get available labels
    ra_throughput_average_sum = sum(ra_throughput_average);
    legend_labels = [];
    for idx_ra = 1:length(ra_filenames)
        if ra_throughput_average_sum(idx_ra) > 0
            legend_labels = [legend_labels, ra_labels{idx_ra}];
        end
    end
    % plot
    for idx_ra = 1:length(ra_filenames)
        if ra_throughput_average_sum(idx_ra) > 0
            plot(holdtime, ra_throughput_average(:, idx_ra), 'color', ra_plot_colors(idx_ra), 'LineWidth',2, 'MarkerSize', 12);
            hold on;
        end
    end
    hold off;
    grid on;
    ylabel('Average Throughput at the End(Mbits/s)');
    xlabel('Hold Time(s)');
    xticks(holdtime);
    legend(legend_labels, 'Location','best');
end