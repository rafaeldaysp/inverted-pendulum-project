clear;
clc;
%% Params

M   = 0.2;          % Mass of cart
m   = 0.075;         % mass of pendulum
l   = 0.147;       % COM of Pendulum
I   = 5.4022e-04;     % MOI of Pendulum
g   = 9.81;         % Gravity Constant
b   = 0.00001;      % damping coefficient
c   = 15;           % friction coefficient

k = 4.81;            % voltage to force

km = 0.04;  % test 
R = 1.7;    % test 
r = 0.0045; %test


%% state space

den = I*(M+m) + M*m*l^2; % denominador comum

A = [0 0 1 0
    0 0 0 1
    0 -m^2*l^2*g/den -(I+m*l^2)*c/den b*m*l/den
    0 m*g*l*(M+m)/den c*m*l/den -(M+m)*b/den];

B = [0
    0
    (I+m*l^2)*k/den
    -m*l*k/ den];
C = [1 0 0 0
    0 1 0 0];

D = 0;

states = {'x' 'theta' 'x_dot' 'theta_dot'};
inputs = {'u'};
outputs = {'x'; 'theta'};

sys_ss = ss(A,B,C,D,'statename',states,'inputname',inputs,'outputname',outputs);

%% discretization

Ts = 0.02;
sys_d = c2d(sys_ss,Ts,'zoh');

%% discrete lqr

Ad = sys_d.a;
Bd = sys_d.b;
Cd = sys_d.c;
Dd = sys_d.d;
q = [40 3 0.05 0.1];
Qd = diag(q);
Rd = 0.01;
[Kd, S, e] = dlqr(Ad,Bd,Qd,Rd);
