PPR = 5120;                    % Pulses per revolution
CPR = 4*PPR;                   % Counts per revolution
pitch_belt = 2;            % 2mm 
N_belt = 20;                   % 40 teeth
d_belt = N_belt*pitch_belt/pi; % pitch diameter
linear_resolution = d_belt*pi/(CPR)