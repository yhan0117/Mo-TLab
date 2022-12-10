clc;clear;clf
%kd, ki, kp
load("MOI.mat")
Kp = 0.5;
Kd = 0.223; %0.22281692032865351726852652442282 is critically damped when kp = 0.5
Ki = 1;

kp = Kp*180/pi; 
kd = Kd*180/pi; 
ki = Ki*180/pi; 

dist = 0;

K = 2*km*O*L/J;

syms u(t)
Du = diff(u,t);
D2u = diff(u,t,2);

ode = diff(u,t,3) == -K*kd*D2u-K*kp*Du-K*ki*u + dist;

V = odeToVectorField(ode);
M = matlabFunction(V,'vars',{'t','Y'});
[t,y] = ode45(M, [0 10], [0 10 0]);
grid on
plot(t,y(:,2))

wn = sqrt(K*kp);
zeta = K*kd/wn/2;

