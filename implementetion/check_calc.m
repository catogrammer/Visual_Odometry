clc

p = [-1 -1 1;
    0 -1 1;
    1 -1 1;
    -1 0 1;
    0 0 1;
    1 0 1;
    -1 1 1;
    0 1 1;
    1 1 1];
p_ = [0 -1 1;
    0 -1/2 1;
    0 -1/3 1;
    0 0 1;
    0 0 1;
    0 0 1;
    0 1/3 1;
    0 1/2 1;
    0 1 1;];


A = [   0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0;
    ];


for i = 1:9
    a = [ p_(i,1)*p(i,1), p_(i,1)*p(i,2), p_(i,1)*p(i,3), p_(i,2)*p(i,1), p_(i,2)*p(i,2), p_(i,2)*p(i,3), p_(i,3)*p(i,1), p_(i,3)*p(i,2), p_(i,3)*p(i,3)];
    A(i, q=1:9) = a(q=1:9);
end

A_2 = [     -0      -0       0       1       1      -1      -1      -1       1;
0      -0       0      -0     0.5    -0.5       0      -1       1;
0      -0       0 -1/3  1/3 -1/3       1      -1       1;
-0       0       0      -0       0       0      -1       0       1;
0       0       0       0       0       0       0       0       1;
0       0       0       0       0       0       1       0       1;
-0       0       0 -1/3  1/3  1/3      -1       1       1;
0       0       0       0     0.5     0.5       0       1       1;
0       0       0       1       1       1       1       1       1;
];

equals_m = A == A_2

C = [1 2 3;
     4 5 6;
     5 7 9]
% e = [1 -2 1]
% C*e'

E = [ 0  0  1;
     0  1 -0;
     1 -0  0 ]
[U, S, V] = svd(E)

U*S*V'

Z = [0 1 0; -1 0 0; 0 0 0]
t_x = U*Z*U'
R = E*inv(t_x)