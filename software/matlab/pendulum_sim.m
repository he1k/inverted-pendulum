%% Setup and parameters
SIM_IDEAL_MEASUREMENT = 1;
SIM_OPEN_LOOP = 1;
STEP_SIZE = 1e-3;
SIM_TIME = 10;
SIM_FILE = "pendulum_model";
run("params.m");

x0 = 0;
x_dot0 = 0;
theta0 = pi;
theta_dot0 = 0;
phi0 = 0;
phi_dot0 = 0;
i0 = 0;
X0 = [theta_dot0;theta0;x_dot0;x0;phi_dot0;phi0;i0];
K_lqr = zeros(1,7);
%% System equations
syms theta dtheta ddtheta x dx ddx F u i di;
% syms I m l g c M b R L J Ke Kt r Kt d

eq(1) = (I+m*l^2)*ddtheta + c*dtheta + m*g*l*sin(theta) == -m*l*ddx*cos(theta);
eq(2) = (M+m)*ddx + b*dx + m*l*ddtheta*cos(theta) - m*l*dtheta^2*sin(theta) == F;
eq(3) = L*di + R*i == u - Ke*dx/r;
eq(4) = J*ddx/r + d*dx/r == Kt*i - F*r;

sol = solve(eq,[ddtheta, ddx, di, F]);
% Extract results
f_ddtheta = simplify(sol.ddtheta);
f_ddx     = simplify(sol.ddx);
f_di      = simplify(sol.di);

X = [dtheta; theta; dx; x; i];
U = u;

% Define the system of first-order ODEs: X_dot = f(X, U)
f = [f_ddtheta; ... % theta_ddot
     dtheta;    ... % theta_dot
     f_ddx;     ... % x_ddot
     dx;        ... % x_dot
     f_di];         % i_dot

Asym = jacobian(f, X);
Bsym = jacobian(f, U);

eq_point = [dtheta, theta, dx, x, i, u];
eq_values = [0, pi, 0, 0, 0, 0];

A = double(subs(Asym, eq_point, eq_values));
B = double(subs(Bsym, eq_point, eq_values));
C = eye(5);
D = 0;
ss_ol = ss(A,B,C,D);
% Display results
fprintf('Linearized A Matrix:\n');
disp(A);
fprintf('Linearized B Matrix:\n');
disp(B);

%% Numerical Example (Optional: Plug in your values)
% val.m = 0.2; val.M = 0.5; val.l = 0.3; val.g = 9.81; ...
% A_num = double(subs(A, val));
%% Linearization
% [An, Bn, Cn, Dn] = linmod(SIM_FILE, X0, 0, 0);
% ss_oln = ss(A, B, C, D);

%% Controller
% create discrete LQR
ss_d = c2d(ss_ol, STEP_CTRL);
F = ss_d.A;
G = ss_d.B;
Q = diag([10, 10000, 0.5,0.1,0])
R = 1;
%Q = diag([10, 1000, 0, 0, 0]); 
R = 0.01; % Make "voltage" cheap so the motor pushes hard
[K_lqr, S, P] = dlqr(F, G, Q, R);

%% Simulate closed loop
% Simulate using discrete model
theta0 = pi - deg2rad(3);
SIM_OPEN_LOOP = 0;
sim("pendulum_simple_model");
%%
theta_dot = rad2deg(logsout.getElement('theta_dot').Values.Data);
theta = rad2deg(logsout.getElement('theta').Values.Data);
% Wrap theta between 180 and -180
theta = mod(theta + 180, 360) - 180;
x = logsout.getElement('x').Values.Data;
x_dot = logsout.getElement('x_dot').Values.Data;
phi = rad2deg(logsout.getElement('phi').Values.Data);
phi_dot = rad2deg(logsout.getElement('phi_dot').Values.Data);
i = logsout.getElement('i').Values.Data;
f_belt = logsout.getElement('f_belt').Values.Data;
u = logsout.getElement('u_sat').Values.Data;
t = 0:STEP_SIZE:SIM_TIME;
figure(1);
ax(1) = subplot(4,1,1);
plot(t,theta, 'r');
xlabel('$t$');
ylabel('$rad$');
legend('$\theta$');
grid on;


ax(2) = subplot(4,1,2);
plot(t,x, 'r');
xlabel('$t$');
ylabel('$m$');
legend('$x$');
grid on;


ax(3) = subplot(4,1,3);
plot(t,phi, 'r');
xlabel('$t$');
ylabel('$A$');
legend('$phi$');
grid on;

ax(4) = subplot(4,1,4);
plot(t,u, 'r'); hold on;
plot(t, f_belt, 'b'); hold off;
xlabel('$t$');
ylabel('$V$');
legend('$u$', '$F$');
grid on;

linkaxes(ax,'x');

%%

% find index for where t >= 3
filename = 'data.csv';
ix = find(t >= 3, 1);

% Build table and write
T = table(t(1:ix), theta(1:ix)', theta_dot(1:ix)', ...
          x(1:ix)', x_dot(1:ix)', phi(1:ix)', phi_dot(1:ix)', i(1:ix)', u(1:ix)',f_belt(1:ix)', ...
          'VariableNames', {'Time', 'theta', 'theta_dot', 'x', 'x_dot', 'phi', 'phi_dot', 'i', 'u', 'f_belt'});
writetable(T, filename);
%%
% figure(1);
% ax(1) = subplot(6,1,1);
% plot(t,theta, 'r', t, phi, 'b');
% xlabel('$t$');
% ylabel('$rad$');
% legend('$\theta$','$\phi$');
% grid on;
% 
% ax(2) = subplot(6,1,2);
% plot(t,theta_dot, 'r', t, phi_dot, 'b');
% xlabel('$t$');
% ylabel('$rad/s$');
% legend('$\dot{\theta}$','$\dot{\phi}$');
% grid on;
% 
% ax(3) = subplot(6,1,3);
% plot(t,x, 'r');
% xlabel('$t$');
% ylabel('$m$');
% legend('$x$');
% grid on;
% 
% ax(4) = subplot(6,1,4);
% plot(t,x_dot, 'r');
% xlabel('$t$');
% ylabel('$m/s$');
% legend('$\dot{x}$');
% grid on;
% 
% ax(5) = subplot(6,1,5);
% plot(t,i, 'r');
% xlabel('$t$');
% ylabel('$A$');
% legend('$i$');
% grid on;
% 
% ax(6) = subplot(6,1,6);
% plot(t,f_belt, 'r');
% xlabel('$t$');
% ylabel('$N$');
% legend('$F_{belt}$');
% grid on;
% 
% linkaxes(ax,'x');

%%
% theta0 = pi - pi/18;
% Ts = 0.001;                                 % sample time (optional)
% x0 = [theta_dot0;theta0;x_dot0;0;phi_dot0;phi0;i0];                   % initial state
% Nsim = 1000;                               % number of steps
% r = 0;                                     % reference (scalar) or vector
% 
% % Preallocate
% nx = size(F,1);
% ny = size(C,1);
% nu = size(G,2);
% x = zeros(nx, Nsim+1);
% y = zeros(ny, Nsim);
% u = zeros(nu, Nsim);
% 
% x(:,1) = x0;
% 
% for k = 1:Nsim
%     % compute control (state feedback), add feedforward if needed
%     u(:,k) = -K_lqr * x(:,k);             % + u_ff if you have one
% 
%     % state update
%     x(:,k+1) = F * x(:,k) + G * u(:,k);
% 
%     % output
%     y(:,k) = C * x(:,k) + D * u(:,k);
% end
% 
% % time vector for plotting
% t = (0:Nsim-1) * Ts;
% 
% % Example plots
% figure;
% subplot(3,1,1); plot(t, y(2,:)); title('Output y'); grid on;
% subplot(3,1,2); plot(t, u'); title('Control u'); grid on;
% subplot(3,1,3); plot((0:Nsim)*Ts, x(2,:)); title('States x'); grid on;