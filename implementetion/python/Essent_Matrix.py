#python3.x

import numpy as np
import json
from scipy.linalg import null_space
# from scipy.linalg import null_space

txt_data = '../input_data/smpldata.txt'
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
def calc_polynom(coord_1, coord_2) :
    count = 0; polynom = np.empty(9)
    for i, el_x2 in enumerate(coord_2):
        for j, el_x1 in enumerate(coord_1):
            polynom[count] = el_x1*el_x2
            count += 1
    return polynom
print('numb of points : ', len(arr_x1))
linear_sys = np.array([])
for i in range(len(arr_x1)):
    if i == 0 :
        linear_sys = np.hstack((linear_sys, calc_polynom(arr_x1[i], arr_x2[i])))
    else :
        linear_sys = np.vstack((linear_sys, calc_polynom(arr_x1[i], arr_x2[i])))

print('Linear sys : \n',linear_sys)
rank = np.linalg.matrix_rank(linear_sys)
print('rank :', rank)

ker = null_space(linear_sys)
print('kernel : \n', ker)

EssentMtrx = np.array([ker[0:3,0], ker[3:6,0], ker[6:9,0]])
print('EssentMtrx : \n', EssentMtrx)

print('Calc u, v : ')
u, s, vh = np.linalg.svd(EssentMtrx, full_matrices=True)

print('u : \n', u)
print('s : \n', s)
print('vh : \n', vh)
w = np.array([[0,1,0],[-1,0,0],[0,0,1]])
R = np.dot(np.dot(u,w.T), vh)
t_x = np.dot(np.dot(np.dot(u,w),np.diag(s)), u.T)
print('Rotation : \n', R)
print('shift : \n', t_x)


# U, s, V = np.linalg.svd(linear_sys,full_matrices=False)
# print("U : ", U)
# print("s : ", s)
# print("V : ", V)
# svd = linalg.svd(a[, full_matrices, compute_uv])
