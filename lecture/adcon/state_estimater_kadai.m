pkg load control

clear()
m = 0.2
l = 0.3
M = 0.3
g = 9.8
J = 1/12*M*l^2

a = (M+m)*(m*l^2+J) -m^2*l^2
A = [0 0 1 0; 0 0 0 1;0 -(m^2*l^2*g/a) 0 0; 0 ((M+m)*m*g*l)/a 0 0]
B = [0 0 (m*l^2+J)/a -(m*l)/a]'
C = [1 0 0 1]

Uc = ctrb(A,B)
Uo = obsv(A,C)

if rank(Uc) > 0 && rank(Uo) > 0
    disp('System is controllable and observable')
else
    disp('System is not controllable and observable')
end

q = [-5 -5 -8 -9]
L = place(A',C',q).'

Q = [100000 0 0 0;0 100000 0 0;0 0 100000 0;0 0 0 100000]
R = 1

K = lqr(A,B,Q,R)

x = [0.1; 0.1; 0.1; 0.1]
xh = [0; 0; 0; 0]

xb0 = [x; xh]

Ab = [A -B*K; L*C A-B*K-L*C]

dt = 0.01
t = 0:dt:7
i = 0

for n = t 
    i = i + 1
    xb = expm(Ab*n)*xb0
    x1(i) = xb(1)
    x2(i) = xb(2)
    x3(i) = xb(3)
    x4(i) = xb(4)
    xh1(i) = xb(5)
    xh2(i) = xb(6)
    xh3(i) = xb(7)
    xh4(i) = xb(8)
endfor


plot(t,x1,t,x2,t,x3,t,x4,t,xh1,'o',t,xh2,'o',t,xh3,'o',t,xh4,'o')
% plot(t,x1,t,x2,t,x3,t,x4)
% plot(t,xh1,'o',t,xh2,'o',t,xh3,'o',t,xh4,'o')
dmy=input('Hit any key to fin ')