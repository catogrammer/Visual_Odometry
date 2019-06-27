#include "headers/Camera.hpp"

void Camera::read_property_cam(std::string path) {
    std::ifstream fin(path, std::ifstream::in);
    Eigen::MatrixXd tmp(3,3);
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            fin >> tmp(i,j);
        }
    }
    this->cam_pose << tmp.row(0).transpose();
    this->normal   << tmp.row(1).transpose();
    this->horizon  << tmp.row(2).transpose();
    fin.close();
}

void Camera::write_to_file(std::string path) {
    std::ofstream fout(path, std::ofstream::app);
    if(!fout.is_open()){
        std::cout << "data doesn't read" << '\n';
    }
    for (size_t i = 0; i < (size_t)this->features.cols(); i++) {
        fout << this->features(0, i) << ' ' << this->features(1, i) << ' ' << this->features(2, i) << '\n';
    }
    fout << '\n';

    fout.close();
}

Eigen::Vector3d projection_on_palne(Eigen::Vector3d normal, Eigen::Vector3d point, double D){
    return point - (((point.dot(normal)) + D) / (normal.dot(normal)))*normal;
}

void Camera::coord_in_cam_space(double D) {
    size_t count_features =  this->features.cols();
    for (size_t i = 0; i < count_features; i++) {
        Eigen::Vector3d b = projection_on_palne(this->normal, this->features.col(i), D);
        // std::cout << "b = " << b.transpose() << '\n';
        this->features.col(i) = b;
    }

    for (size_t i = 0; i < count_features; i++) {
        Eigen::Vector3d el = this->features.col(i);
        this->features.col(i) = Eigen::Vector3d(el[0]/el[2], el[1]/el[2], el[2]/el[2]);
    }

}
