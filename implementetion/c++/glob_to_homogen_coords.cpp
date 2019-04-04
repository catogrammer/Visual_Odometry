#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <string>
#include <eigen3/Eigen/Dense>
#include "Camera.hpp"

Eigen::MatrixXd read_glob_coord(std::string path) {
    std::ifstream fin(path);
    if(!fin){
        std::cout << "data doesn't read" << '\n';
    }
    size_t count_features = 0;
    fin >> count_features;

    Eigen::MatrixXd features(3,count_features);
    for (size_t i = 0; i < count_features; i++)
        for (size_t j = 0; j < 3; j++)
            fin >> features(j,i);

    fin.close();
    return features;
}

void read_data_def_cam(Camera &cam, std::string path) {
    std::ifstream fin(path, std::ifstream::in);
    Eigen::MatrixXd tmp(3,3);
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            fin >> tmp(i,j);
        }
    }
    cam.cam_pose << tmp.row(0).transpose();
    cam.normal   << tmp.row(1).transpose();
    cam.horizon  << tmp.row(2).transpose();

    fin.close();
}

void write_to_file(Eigen::MatrixXd features, std::string path) {
    std::ofstream fout(path, std::ofstream::app);
    if(!fout.is_open()){
        std::cout << "data doesn't read" << '\n';
    }
    for (size_t i = 0; i < features.cols(); i++) {
        fout << features(0, i) << ' ' << features(1, i) << ' ' << 1 << '\n';
    }
    fout << '\n';

    fout.close();
}

/*
    p_01 = ((r_0 * n_1) + D1) / |n_1|
    r_1 = r_0 - p_01/|n_1| * n_1
*/

Eigen::Vector3d projection_on_palne(Eigen::Vector3d normal, Eigen::Vector3d point, double D){
    return point - (((point.dot(normal)) + D) / (normal.dot(normal)))*normal;
}

Eigen::MatrixXd transform_to_homogen(Camera const &cam, double D, Eigen::MatrixXd featur_3d) {
    size_t count_features =  featur_3d.cols();
    Eigen::MatrixXd res(2, count_features);
    Eigen::MatrixXd A(3,2);
    A.col(0) = cam.horizon;
    A.col(1) = cam.vertical;
    for (size_t i = 0; i < count_features; i++) {
        Eigen::Vector3d b(projection_on_palne(cam.normal, featur_3d.col(i), D));
        res.col(i) = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
        // res.col(i) = A.householderQr().solve(b);
    }
    return res;
}

int main(int argc, char const *argv[]) {
    std::string path_glob_c = "../input_data/global_coords.txt";
    std::string path_cam_def = "../input_data/def_cam_data.txt";
    Eigen::MatrixXd glob_features = read_glob_coord(path_glob_c);
    std::cout << "feaures : \n" << glob_features << '\n';
    Camera camera;
    read_data_def_cam(camera, path_cam_def);
    camera.vertical = camera.horizon.cross(camera.normal).transpose();
    std::cout << "Cam data : "
              << "\npose : " << camera.cam_pose.transpose()
              << "\nnormal : "   << camera.normal.transpose()
              << "\nhorizon : "  << camera.horizon.transpose()
              << "\nvertical : " << camera.vertical.transpose() << '\n';
    Eigen::Vector3d point(1, 2, 3);
    // Eigen::Vector3d tmp_feat = projection_on_palne(camera.normal, point, 0);
    // std::cout << "Proj coord : " << tmp_feat.transpose() << '\n';
    double D = (-1)*(camera.normal.dot(camera.cam_pose));
    Eigen::MatrixXd loc_features = transform_to_homogen(camera, D, glob_features);
    std::cout << "local features : \n" << loc_features << '\n';

    std::string path_homo_f = "../input_data/homogen_features.txt";
    std::ofstream fout(path_homo_f, std::ofstream::out);
    fout << loc_features.cols() << '\n';
    fout.close();
    write_to_file(loc_features, path_homo_f);

    return 0;
}
