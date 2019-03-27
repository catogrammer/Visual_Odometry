#python3.x

import numpy as np
import json
from scipy.linalg import null_space


txt_data = './test_data.txt'
json_data = 'data_coord.json'


file1 = open(txt_data,"r")
data = file1.read()
file1.close()
numb_f = int(data.split('\n')[0])
arr = data.split('\n')[1::]
my_array2 = np.genfromtxt(arr, delimiter=" ")

features_1 = my_array2[0:numb_f,:]
features_2 = my_array2[numb_f:2*numb_f,:]

print(features_1)
print(features_2)


arr_x1 = np.array(features_1)
arr_x2 = np.array(features_2)

# x_1*E*x_2
# [ xx', yx', x', xy', yy', y', x, y, 1 ]

def calc_polynom(arr_x1, arr_x2) :
    polynom = np.array([(arr_x1[0]*arr_x2[0]), arr_x1[1]*arr_x2[0],
    arr_x2[0], arr_x1[0]*arr_x2[1], arr_x1[1]*arr_x2[1], arr_x2[1],
    arr_x1[0], arr_x1[1], 1])
    return polynom

# print('numb of points : ', len(arr_x1))
linear_sys = np.array([])
for i in range(len(arr_x1)):
    if i == 0 :
        linear_sys = np.hstack((linear_sys, calc_polynom(arr_x1[i], arr_x2[i])))
    else :
        linear_sys = np.vstack((linear_sys, calc_polynom(arr_x1[i], arr_x2[i])))
print('[ xx\', yx\', x\', xy\', yy\', y\', x, y, 1 ] ')
print('Linear sys : \n ',linear_sys)



print('Calc u, v : ')
u, s, vh = np.linalg.svd(linear_sys, full_matrices=True)

print('u : \n', u)
print('s : \n', s)
print('vh : \n', vh)
solve = vh[:,8]
print('SOLVE : ', solve)

EssentMtrx = np.array([vh[0:2,8],vh[3:5,8],vh[6:8,8]])
print('E = \n', EssentMtrx)
rank = np.linalg.matrix_rank(EssentMtrx)
print('rank :', rank)

print('Calc u, v : ')
u_e, s_e, vh_e = np.linalg.svd(EssentMtrx, full_matrices=True)
#
print('u_e : \n', u_e)
print('s_e : \n', s_e)
print('vh_e : \n', vh_e)
w_T = np.array([[0,1,0],[-1,0,0],[0,0,1]])
R = np.dot(np.dot(u_e, w_T), vh_e)
# t_x = np.dot(np.dot(np.dot(u,w),np.diag(s)), u.T)
# print('Rotation : \n', R)
# print('shift : \n', t_x)


# U, s, V = np.linalg.svd(linear_sys,full_matrices=False)
# print("U : ", U)
# print("s : ", s)
# print("V : ", V)
# svd = linalg.svd(a[, full_matrices, compute_uv])
