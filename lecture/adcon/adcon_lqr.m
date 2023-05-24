pkg load control
A = [0 1;-5 -6]
B = [0;1]
Q = [10000 0;0 1000]
R = 1
x0 = [1;0]
K = lqr(A,B,Q,R)

t = 0:0.03:6
i = 0
x1(1) = x0(1)
x2(1) = x0(2)
n1(1) = x0(1)
n2(1) = x0(2)
for n = t
    i = i + 1
    tmpx = expm(A * n - K * B * n) * x0 
    nx = expm(A * n) * x0
    x1(i) = tmpx(1)
    x2(i) = tmpx(2)
    n1(i) = nx(1)
    n2(i) = nx(2)
endfor
plot(t,x1,t,x2,t,n1,t,n2)
K
dmy=input('Hit any key to fin ')