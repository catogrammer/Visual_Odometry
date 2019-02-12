#python3.x

import numpy as np
import json
import scipy
from scipy.linalg import null_space

txt_data = 'smpldata.txt'
json_data = 'data_coord.json'

#read data
f = open(json_data, 'r')
data = f.read()
f.close()

# parse data
coords = json.loads(data)
features_1 = coords['x1']
features_2 = coords['x2']
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

linear_sys = np.array([])
for i in range(len(arr_x1)):
    if i == 0 :
        linear_sys = np.hstack((linear_sys, calc_polynom(arr_x1[i], arr_x2[i])))
    else :
        linear_sys = np.vstack((linear_sys, calc_polynom(arr_x1[i], arr_x2[i])))

print(linear_sys)
rank = np.linalg.matrix_rank(linear_sys)
print(rank)


ker_of_sys = null_space(linear_sys)
print(ker_of_sys)

#
# U, s, V = np.linalg.svd(linear_sys,full_matrices=False)
# print("U : ", U)
# print("s : ", s)
# print("V : ", V)
# svd = linalg.svd(a[, full_matrices, compute_uv])






# Your data in the text file
# Value1  Value2  Value3
# 0.4839  0.4536  0.3561
# 0.1292  0.6875  MISSING
# 0.1781  0.3049  0.8928
# MISSING 0.5801  0.2038
# 0.5993  0.4357  0.7410

# my_array2 = np.genfromtxt('data2.txt', skip_header=1, filling_values=-999)

# parsing from txt

# data = data.replace('\n', ' ')
# data = data.split(' ')
# print(data, end='')
# data = ' '.join(data).split(' ') # Самое быстрое решение для удаления ''
# и пустых строк с пробелом ' ' остается
# print(len(data))
# for i, el in enumerate(features_1):
#     print(el, end='')
#     data[i] = float(el)
