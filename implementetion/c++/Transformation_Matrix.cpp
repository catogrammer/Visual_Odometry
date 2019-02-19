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
    for (size_t i = 0; i < 9; i++) {
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
    // camera_1.cam_pose  = camera_1.cam_pose.normalized();
    // camera_1.normal    = camera_1.normal.normalized(); // y
    // camera_1.horizon   = camera_1.horizon.normalized(); // x
    //
    // camera_2.cam_pose  = camera_2.cam_pose.normalized();
    // camera_2.normal    = camera_2.normal.normalized(); //loc in cam pose coord
    // camera_2.horizon   = camera_2.horizon.normalized(); //loc in cam pose coord
    // //
    camera_1.features = Eigen::MatrixXd(3,9); //(row,column)
    camera_2.features = Eigen::MatrixXd(3,9);

    // std::cout << "Dot product normal and horizont for camera 1 : "
    // << camera_1.normal.dot(camera_1.horizon) << '\n';
    // std::cout << "Dot product normal and horizont for camera 2 : "
    // << camera_2.normal.dot(camera_2.horizon) << '\n';

    // if( camera_1.normal.dot(camera_1.horizon) ||  camera_2.normal.dot(camera_2.horizon)){
    //     std::cout << "Bad data, programm is stoped!" << '\n';
    //     exit(0);
    // }

    //********************
    std::srand(std::time(0)); //std::rand() % left_range - right_range

    // for (size_t i = 0; i < 9; i++) {
    //     camera_1.features.col(i) << (std::rand() % (-2) - 2), 3, (std::rand() % (-2) - 2);
    //     /*
    //         постоянной должно быть направление номали
    //     */
    // }

    camera_1.features.col(0) << 0, 3, 0;
    camera_1.features.col(1) << -2, 3, 2,
    camera_1.features.col(2) << -1, 3, 2,
    camera_1.features.col(3) << -2, 3, -2,
    camera_1.features.col(4) << -1, 3, -2,
    camera_1.features.col(5) << 1, 3, -2,
    camera_1.features.col(6) << 2, 2, -2,
    camera_1.features.col(7) << 1, 3, 2,
    camera_1.features.col(8) << 2, 3, 2;

    std::cout << "*****************features - 1***********************" << '\n';
    std::cout << camera_1.features << '\n';
    std::cout << "****************************************************" << '\n';

    //find y'
    camera_1.vertical << camera_1.horizon.cross(camera_1.normal);
    camera_2.vertical << camera_2.horizon.cross(camera_2.normal);

    std::cout << "vertical : " << camera_2.vertical  << '\n';

    /*
        нормировать все координаты, которые мы задаем (x, y, z)
    */

    //find transformation matrix
    Eigen::Matrix3d transf_m_1;
    Eigen::Matrix3d transf_m_2;
    transf_m_1 << camera_1.horizon, camera_1.normal, camera_1.vertical;
    transf_m_2 << camera_2.horizon, camera_2.normal, camera_2.vertical;
    // transf_m_2 << camera_2.horizon, camera_2.normal, Eigen::Vector3d({0, 0, 1});

    //transform X_local_1 to X_glob : X_glob = M_1*X_loc + C_1
    Eigen::MatrixXd features_in_glob(3,9);
    for (size_t i = 0; i < features_in_glob.cols(); i++) {
        features_in_glob.col(i) << (transf_m_1 * camera_1.features.col(i) + camera_1.cam_pose);
    }

    std::cout << "Global features : \n" << features_in_glob << '\n';

    //transform X_glob to X_local_2 : X_local_2 = M_2^(-1)(X_glob - C_2)
    std::cout << "matrix transformation m 2 : \n" << transf_m_2 << '\n';
    for (size_t i = 0; i < 9; i++) {
        camera_2.features.col(i) << transf_m_2.inverse() * (features_in_glob.col(i) - camera_2.cam_pose);
    }

    std::cout << "*****************features - 2***********************" << '\n';
    std::cout << camera_2.features << '\n';
    std::cout << "****************************************************" << '\n';

    std::string path = "./src/input_data/smpldata.txt";
    std::ofstream fout(path, std::ofstream::out);
    fout.close();
    write_to_file(camera_1.features, path);
    write_to_file(camera_2.features, path);


    return 0;
}
