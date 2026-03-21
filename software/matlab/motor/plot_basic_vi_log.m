% Filename of the motor voltage / current log
fname = 'basic_vi_log.txt';  % change to your file name

% Read numeric data; automatically handles CSV-style commas
M = readmatrix(fname);

% If file has a non-numeric header row, uncomment the next line:
% M = readmatrix(fname, 'NumHeaderLines', 1);

% Expect three columns: time, volt, current
time   = M(:,1);
volt   = M(:,2);
current= M(:,3);

time = time - time(1); % Remove bias
d = diff(time);
% Calculate min, max and mean of variable d
minD = min(d);
maxD = max(d);
meanD = mean(d);
s = "Min time diff: " + minD + ", max time diff: " + maxD + ", mean time diff: " + meanD;
disp(s)

time = time*1e-3;

% Theoretical brushed DC motor
R = 2.59;     % Resistance (ohm)
L = 708e-6;   % Inductance (H)
J = 115*1e-7; % Rotor inertia (kg m^3) 
Kt = 79.7e-3; % Torque constant (Nm)
Ke = Kt;      % EMF constant V/(rad/s)
B = 0;        % Viscous friction (Nm/(rad/s))
Mr = 6.5e-3;
% Create transfer function for u/i where u is motor voltage and i is
% current - For theoretical system
s = tf('s');
G = (J*s + b)/((L*s + R)*(J*s +b)+Kt*Ke);
%s_tf = tf(Kt, [L R]); % Create transfer function for the motor

% Ensure volt and time are column vectors matching expected dimensions
volt = volt(:);
time = time(:);

% Simulate the response of the system
y = lsim(G, -volt, time); % Simulate the response of the system

% EXPERIMENTAL (GPT stuff lol)
% Tc = 6.5e-3;        % Coulomb friction torque (Nm), from MR
% 
% % states: x = [i; w]
% % di = (u - R i - Ke w)/L
% % dw = (Kt i - Tc*sign(w) - Tload)/J   (set Tload=0 for now)
% 
% dt = mean(diff(time));
% i = 0; w = 0;
% y = zeros(size(time));
% 
% for k = 1:numel(time)
%     u = volt(k);
% 
%     % sign(w) with a deadband to avoid chattering near 0
%     if abs(w) < 1e-3, sgn = 0; else, sgn = sign(w); end
% 
%     di = (u - R*i - Ke*w)/L;
%     dw = (Kt*i - Tc*sgn)/J;
% 
%     i = i + dt*di;
%     w = w + dt*dw;
% 
%     y(k) = i;
% end


mdl = 'motor_model.slx'
u_sim = [time,-volt];

out = sim(mdl, 'StopTime', string(time(end)));
y_sim = out.get('i_sim');
% Plot
figure;
yyaxis left
plot(time, volt, '-g', 'LineWidth', 1.5);
ylabel('Motor voltage $[V]$');

yyaxis right
stairs(time, current, '-r', 'LineWidth', 1.5); hold on;
plot(time, y_sim, '--b', 'LineWidth', 1.5);
plot(time, y, '--m', 'LineWidth', 1.5); hold off;
ylabel('Motor current $[A]$');

xlabel('Time $[s]$');
title('Voltage and Current vs Time');
legend('$u$','$i_{meas}$ ', '$i_{sim,sim}$','$i_{sim,mat}$');
grid on;