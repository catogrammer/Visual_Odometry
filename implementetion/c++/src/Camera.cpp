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

void Camera::get_homogen_coord() {
    size_t count_features =  this->features.cols();
    Eigen::Matrix3d transf_m;
    transf_m << horizon, vertical, normal;
    for (size_t i = 0; i < count_features; i++) {
        Eigen::Vector3d el = this->features.col(i);
        this->features.col(i) = Eigen::Vector3d(el[0]/el[1], el[2]/el[1], el[1]/el[1]);

        // this->features.col(i) = Eigen::Vector3d(el[0]/el[1], el[1]/el[1], el[2]/el[1]);
        // this->features.col(i) = Eigen::Vector3d(el[0], el[1], el[2]);

        /*
        // this->features.col(i) = transf_m*(Eigen::Vector3d(el[0]/el[2], el[1]/el[2], el[2]/el[2])) + cam_pose;
        // geting global position coordinates points in camera Coordinate.
        */

    }
}

void Camera::transform_featutes() {
    this->vertical << this->horizon.cross(this->normal); // == x * n

    // normalisation of coordinates
    // this->normal    = this->normal.normalized(); // y
    // this->horizon   = this->horizon.normalized(); // x
    // this->vertical   = this->horizon.normalized(); //z

    Eigen::Matrix3d transf_m;
    transf_m << this->horizon, this->vertical, this->normal;

    // std::cout << "Transform matrix \n" << transf_m <<'\n';

    std::cout << "size = " << this->features.cols() << '\n';
    for (size_t i = 0; i < (size_t)features.cols(); i++) {
        // std::cout << "el = " << features.col(i) << '\n';
        this->features.col(i) << transf_m.inverse() * (this->features.col(i) - this->cam_pose);
    }

}
