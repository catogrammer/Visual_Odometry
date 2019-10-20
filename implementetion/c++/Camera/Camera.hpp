#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Eigen/Dense>
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
    bool is_local_features = false;

    Camera(){}

    void read_property_cam(std::string path);

    void write_to_file(std::string path);

    void get_homogen_coord();

    void transform_featutes_to_local_coord(); //to local coord

    void transform_featutes_to_global_coord(); //to local coord

};

#endif
