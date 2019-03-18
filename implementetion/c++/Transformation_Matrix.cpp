#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

#include <eigen3/Eigen/Dense>

struct Camera {
    Eigen::Vector3d cam_pose; //position of cameras in global coord
    Eigen::Vector3d normal; // cameras direction //view direction in global coord
    Eigen::Vector3d horizon; // right vector or x' in global coord
    Eigen::Vector3d vertical; // left vector or y' in global coord
    Eigen::MatrixXd features; // in local coordinate
};

void write_to_file(Eigen::MatrixXd features, std::string path) {
    std::ofstream fout(path, std::ofstream::app);
    if(!fout.is_open()){
        std::cout << "data doesn't read" << '\n';
    }
    for (size_t i = 0; i < features.cols(); i++) {
        fout << features(0, i) << ' ' << features(1, i) << ' ' << features(2, i) << '\n';
    }
    fout << '\n';

    fout.close();
}

// reading data from file definition carachteristics cam
void read_data_def_cam(Camera &cam_1, Camera &cam_2, std::string path) {
    std::ifstream fin(path, std::ifstream::in);
    Eigen::MatrixXd tmp(6,3);
    double a;
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 3; j++) {
            fin >> tmp(i,j);
        }
    }
    cam_1.cam_pose << tmp.row(0).transpose();
    cam_1.normal   << tmp.row(1).transpose();
    cam_1.horizon  << tmp.row(2).transpose();
    //
    cam_2.cam_pose << tmp.row(3).transpose();
    cam_2.normal   << tmp.row(4).transpose(); //loc in cam pose coord
    cam_2.horizon  << tmp.row(5).transpose();

    fin.close();
}

int main(int argc, char const *argv[]) {
    Camera camera_1;
    Camera camera_2;

    read_data_def_cam(camera_1, camera_2, "./def_cam_data.txt");

    // normalisation of coordinates
    camera_1.normal    = camera_1.normal.normalized(); // y
    camera_1.horizon   = camera_1.horizon.normalized(); // x
    camera_2.normal    = camera_2.normal.normalized(); //loc in cam pose coord
    camera_2.horizon   = camera_2.horizon.normalized(); //loc in cam pose coord
    //
    camera_1.features = Eigen::MatrixXd(3,11); //(row,column)
    camera_2.features = Eigen::MatrixXd(3,11);

    //********************
    std::srand(std::time(0)); //std::rand() % left_range - right_range

    // for (size_t i = 0; i < 9; i++) {
    //     camera_1.features.col(i) << (std::rand() % (-2) - 2), 1, (std::rand() % (-2) - 2);
    //     /*
    //         постоянной должно быть направление номали
    //     */
    // }
    // (horizon - x_pixel, normal - deepth, vertical - y_pixel)
    camera_1.features.col(0) << 0, 0, 3;
    camera_1.features.col(1) << 1, 1, 3,
    camera_1.features.col(2) << 1, 2, 3,
    camera_1.features.col(3) << 2, 2, 3,
    camera_1.features.col(4) << -1, 1, 3,
    camera_1.features.col(5) << -1, 2, 3,
    camera_1.features.col(6) << -2, 2, 3,
    camera_1.features.col(7) << 0, 1, 3,
    camera_1.features.col(8) << 0, 2, 3;
    camera_1.features.col(9) << 2, 3, 3;
    camera_1.features.col(10) << 3, 2, 3;

    std::cout << "*****************features - 1***********************" << '\n';
    std::cout << camera_1.features << '\n';
    std::cout << "****************************************************" << '\n';

    //find y'
    camera_1.vertical << camera_1.horizon.cross(camera_1.normal); // == x * n
    camera_2.vertical << camera_2.horizon.cross(camera_2.normal);

    //find transformation matrix
    Eigen::Matrix3d transf_m_1;
    Eigen::Matrix3d transf_m_2;
    transf_m_1 << camera_1.horizon, camera_1.vertical, camera_1.normal;
    transf_m_2 << camera_2.horizon, camera_2.vertical, camera_2.normal;

    //transform X_local_1 to X_glob : X_glob = M_1*X_loc + C_1
    Eigen::MatrixXd features_in_glob(3,camera_1.features.cols());
    for (size_t i = 0; i < features_in_glob.cols(); i++) {
        features_in_glob.col(i) << (transf_m_1 * camera_1.features.col(i) + camera_1.cam_pose);
    }
    std::cout << "Global features : \n" << features_in_glob << '\n';

    //transform X_glob to X_local_2 : X_local_2 = M_2^(-1)(X_glob - C_2)
    for (size_t i = 0; i < camera_1.features.cols(); i++) {
        camera_2.features.col(i) << transf_m_2.inverse() * (features_in_glob.col(i) - camera_2.cam_pose);
    }

    std::cout << "*****************features - 2***********************" << '\n';
    std::cout << camera_2.features << '\n';
    std::cout << "****************************************************" << '\n';

    std::string path = "../input_data/smpldata.txt";
    std::ofstream fout(path, std::ofstream::out);
    fout << camera_1.features.cols() << '\n';
    fout.close();

    // std::ofstream fout_(path, std::ofstream::app);
    // fout_ << camera_1.features.cols();
    // fout_.close();
    write_to_file(camera_1.features, path);
    write_to_file(camera_2.features, path);


    return 0;
}
