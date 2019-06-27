#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <fstream>
#include <ostream>

class Camera {
public:
    Eigen::Vector3d cam_pose; //position of cameras in global coord
    Eigen::Vector3d normal; // cameras direction //view direction in global coord
    Eigen::Vector3d horizon; // right vector or x' in global coord
    Eigen::Vector3d vertical; // left vector or y' in global coord
    Eigen::MatrixXd features; // in local coordinate

    Camera(){}

    void read_property_cam(std::string path);

    void write_to_file(std::string path);

    void coord_in_cam_space(double D);

};

#endif
