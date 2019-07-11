# libraries
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


# Dataset
# df=pd.DataFrame({'X': range(-4,5), 'Y': np.random.randn(9)*8+range(-4,5), 'Z': (np.random.randn(9)*8+range(-4,5))*2 })

cam_1_pose = np.array([1,1,1])
normal_1   = np.array([1,1,0])+cam_1_pose
horison_1  = np.array([1,-1,0])+cam_1_pose
vertical_1 = np.array([0,0,1])+cam_1_pose

cam_2_pose = np.array([4,2,1])
normal_2   = np.array([3,3,1])+cam_2_pose
horison_2  = np.array([3,1,1])+cam_2_pose
vertical_2 = np.array([0,0,1])+cam_2_pose

def read_features_in_coord_cam():
    with open('../input_data/features.txt') as f:
        features = f.read()
    data = features.split('\n', 1)
    size = int(data[0])
    feat = data[1].split('\n')
    feat = [el for el in feat if el] # delete empty str
    feat = [el.split(' ') for el in feat]

    arr_p = np.array([[1,1,1]]) # for delete after append
    for el in feat:
        arr_p = np.append(arr_p, [[float(k) for k in el if k]], axis=0) #cast to float each element

    res = np.delete(arr_p, 0, axis=0)
    first_p = res[0:size]
    second_p = res[size:]
    return (first_p, second_p)

def read_global_features():
    with open('../input_data/global_coords.txt') as f:
        features = f.read()
    data = features.split('\n', 1)
    size = int(data[0])
    feat = data[1].split('\n')
    feat = [el for el in feat if el] # delete empty str
    feat = [el.split(' ') for el in feat]

    arr_p = np.array([[1,1,1]]) # for delete after append
    for el in feat:
        arr_p = np.append(arr_p, [[float(k) for k in el if k]], axis=0) #cast to float each element

    res = np.delete(arr_p, 0, axis=0)
    return res
    # first_p = res[0:size]
    # second_p = res[size:]
    # return (first_p, second_p)


def draw_axis(l_bord, r_bord, color):
    xAxisLine = ((l_bord, r_bord), (0, 0), (0,0))
    ax.plot(xAxisLine[0], xAxisLine[1], xAxisLine[2], color, alpha=.5)
    yAxisLine = ((0, 0), (l_bord, r_bord), (0,0))
    ax.plot(yAxisLine[0], yAxisLine[1], yAxisLine[2], color, alpha=.5)
    zAxisLine = ((0, 0), (0,0), (l_bord, r_bord))
    ax.plot(zAxisLine[0], zAxisLine[1], zAxisLine[2], color, alpha=.5)

def draw_cam_axis(cam_pose, normal, horison, vertical, color):
    xAxisLine = ((cam_pose[0], normal[0]), (cam_pose[1], normal[1]), (cam_pose[2], normal[2]))
    ax.plot(xAxisLine[0], xAxisLine[1], xAxisLine[2], color, alpha=.5)
    yAxisLine = ((cam_pose[0], horison[0]), (cam_pose[1], horison[1]), (cam_pose[2], horison[2]))
    ax.plot(yAxisLine[0], yAxisLine[1], yAxisLine[2], color, alpha=.5)
    zAxisLine = ((cam_pose[0], vertical[0]), (cam_pose[1], vertical[1]), (cam_pose[2], vertical[2]))
    ax.plot(zAxisLine[0], zAxisLine[1], zAxisLine[2], color, alpha=.5)

def draw_line_proj(beg, end, color='c'):
    Line = ((beg[0], end[0]), (beg[1], end[1]), (beg[2], end[2]))
    ax.plot(Line[0], Line[1], Line[2], color, alpha=.5)


# plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

l_bord = -7
r_bord = 7

draw_axis(l_bord, r_bord, 'r')
draw_cam_axis(cam_1_pose, normal_1, horison_1, vertical_1, 'g')
draw_cam_axis(cam_2_pose, normal_2, horison_2, vertical_2, 'black')
features_in_cam  = read_features_in_coord_cam()
global_coords = read_global_features()
print(global_coords)
print(features_in_cam)
# print(features_in_cam[1][:, 0])


df_1=pd.DataFrame({'X': features_in_cam[0][:,0], 'Y': features_in_cam[0][:,2], 'Z': features_in_cam[0][:,1] })
df_2=pd.DataFrame({'X': features_in_cam[1][:,0], 'Y': features_in_cam[1][:,2], 'Z': features_in_cam[1][:,1] })
df_glob=pd.DataFrame({'X': global_coords[:,0], 'Y': global_coords[:,1], 'Z': global_coords[:,2] })


ax.scatter(df_1['X'], df_1['Y'], df_1['Z'], c='red', s=80)
ax.scatter(df_2['X'], df_2['Y'], df_2['Z'], c='skyblue', s=80)
ax.scatter(df_glob['X'], df_glob['Y'], df_glob['Z'], c='magenta', s=80)

for i in range(len(global_coords)):
    # draw_line_proj(global_coords[i], [features_in_cam[0][i,0], features_in_cam[0][i,1], features_in_cam[0][i,2]] )
    draw_line_proj(global_coords[i], cam_1_pose )

    draw_line_proj(global_coords[i], [features_in_cam[1][i,0], features_in_cam[1][i,2], features_in_cam[1][i,1]] )



ax.view_init(30, 185)
plt.show()
