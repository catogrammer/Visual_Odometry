#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <string>
#include <eigen3/Eigen/Dense>
#include "Camera.hpp"

int main(int argc, char const *argv[]) {
    std::string path_glob_c = "../input_data/global_coords.txt";
    std::string path_cam_def = "../input_data/def_cam_data.txt";

    Camera *camera = new Camera();
    camera->read_property_cam(path_cam_def);
    camera->read_features(path_glob_c);

    camera->vertical = camera->horizon.cross(camera->normal).transpose();
    double D = (-1)*(camera->normal.dot(camera->cam_pose));

    std::cout << "global features : \n" << camera->features << '\n';
    std::cout << "Cam data : "
              << "\npose : " << camera->cam_pose.transpose()
              << "\nnormal : "   << camera->normal.transpose()
              << "\nhorizon : "  << camera->horizon.transpose()
              << "\nvertical : " << camera->vertical.transpose() << '\n';


    camera->transform_to_homogen(D);
    std::cout << "local features : \n" << camera->features  << '\n';

    std::string path_homo_f = "../input_data/homogen_features.txt";
    std::ofstream fout(path_homo_f, std::ofstream::out);
    fout << camera->features.cols() << '\n';
    fout.close();

    camera->write_to_file(path_homo_f);

    return 0;
}
