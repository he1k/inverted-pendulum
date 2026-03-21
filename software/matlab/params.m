%% Encoder
PPR = 5120;                            % Pulses per revolution
CPR = 4*PPR;                           % Counts per revolution
PITCH_B = 2e-3;                           % 2mm 
PULLEY_T = 20;                         % 40 teeth
PULLEY_D = PULLEY_T*PITCH_B/pi;        % pitch diameter
LINEAR_QUANTIZATION = PULLEY_D*pi/(CPR);% encoder quantization (linear case)
ANGULAR_QUANTIZATION = 2*pi/(CPR);      % encoder quantization (angular case)
%% Current sensing
VREF = 3.3; % volt
MID_REF = VREF/2;
VOLT_PER_AMP = 0.3; % V/A
ADC_RES = 12; % 12 bits
ADC_MAX = 2^ADC_RES; % Maximum ADC reading
ADC_QUANTIZATION = VREF/ADC_MAX; % adc volt quantization
CURRENT_QUANTIZATION = ADC_QUANTIZATION*1/VOLT_PER_AMP; % adc current quantization
CURRENT_SATURATION = MID_REF/VOLT_PER_AMP; % max current to read
%% Motor driver
VOLTAGE_SATURATION = 12; % volt

%% Controller
STEP_CTRL = 1e-3;

%% Model
% Motor
R = 2.59;     % Resistance (ohm)
L = 708e-6;   % Inductance (H)
J = 115*1e-7; % Rotor inertia (kg m^3) 
Kt = 79.7e-3; % Torque constant (Nm)
Ke = Kt;      % EMF constant V/(rad/s)
d = 0;        % Viscous friction (Nm/(rad/s))
Mr = 6.5e-3;  % Friction torque (Nm)
N = 1; % Gear ratio

% Misc
r = PULLEY_D/2; % Pulley radius (m)
g = 9.82;       % Gravitational constant (m/s^2)
% Pendulum + cart
Ltotal = 0.4; % Pendulum length (m)
l = Ltotal/2; % Pendulum half length (m)
M = 54e-3; % Cart mass (kg)
m = 7850*Ltotal*(4e-3)^2*pi; % Pendulum mass (kg)
I = (1/3) * m * l^2; % Pendulum inertia (kgm^2)
b = 0; % friction coefficient trolley [kg/s]
c = 0; % friction coefficient pendulum [kg*m^2/s]
% Belt
k_belt = 1e3; % Belt stiffness (N/m)
Meff = (M+m)+J/r^2;
zeta = 0.7;
c_belt = 2*zeta*sqrt(Meff*k_belt); % Belt dampening (Ns/m)