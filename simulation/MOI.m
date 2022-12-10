%%
%measured
clear;
clc;
T = 1.4;
w = 2*pi/T;
P = w^2;

kp = 0.5*180/pi;
km = 0.0000040990;
O = 300;
L = 25/100;

syms moi

J = double(vpa(solve(P == kp*2*km*O*L/moi)));

%calc
m = 28/1000;
r = 25/100/2;
moi2 = 2*m*r^2;

save('MOI.mat','J', 'km', 'O', 'L')
