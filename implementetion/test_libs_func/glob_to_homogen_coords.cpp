#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <string>
#include <eigen3/Eigen/Dense>
#include "Camera.hpp"

int main(int argc, char const *argv[]) {
    Camera* cam_1 = new Camera();
    Camera* cam_2 = new Camera();

    cam_1->read_property_cam("../input_data/def_cam_1_data.txt");
    cam_2->read_property_cam("../input_data/def_cam_2_data.txt");

    Eigen::MatrixXd orig_features = read_features("../input_data/global_coords.txt");

    cam_1->features = orig_features;
    cam_2->features = orig_features;

    cam_1->transform_featutes();
    cam_2->transform_featutes();

    cam_1->get_homogen_coord();
    cam_2->get_homogen_coord();

    return 0;
}
