#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
// #include <utility>
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

int main(int argc, char const *argv[]) {
    Camera camera_1;
    Camera camera_2;
    //set global coordinate;
    camera_1.cam_pose << 2, 2, 0;
    camera_1.normal   << 2, 2, 1;
    camera_1.horizon  << 3, -2, 0;

    camera_2.cam_pose << 0, 3, 2;
    camera_2.normal   << 0, -5, 0;
    camera_2.horizon  << 3, 2, 1;

    camera_1.features = Eigen::MatrixXd(3,9); //(row,column)
    camera_2.features = Eigen::MatrixXd(3,9);
    //********************
    std::srand(std::time(0)); //std::rand() % left_range - right_range

    for (size_t i = 0; i < 9; i++) {
        camera_1.features.col(i) << (std::rand() % (-5) - 10), (std::rand() % (-5) - 10),  3;
    }

    std::cout << "*****************features - 1***********************" << '\n';
    std::cout << camera_1.features << '\n';
    std::cout << "****************************************************" << '\n';


    //find y'
    camera_1.vertical << camera_1.horizon.cross(camera_1.normal);
    camera_2.vertical << camera_2.horizon.cross(camera_2.normal);

    //find transformation matrix
    Eigen::Matrix3d transf_m_1;
    Eigen::Matrix3d transf_m_2;
    transf_m_1 << camera_1.horizon, camera_1.vertical, camera_1.normal;
    transf_m_2 << camera_2.horizon, camera_2.vertical, camera_2.normal;

    //transform X_local_1 to X_glob : X_glob = M_1*X_loc + C_1
    Eigen::MatrixXd features_in_glob(3,9);
    for (size_t i = 0; i < 9; i++) {
        features_in_glob.col(i) << (transf_m_1 * camera_1.features.col(i) + camera_1.cam_pose);
    }

    //transform X_glob to X_local_2 : X_local_2 = M_2^(-1)(X_glob - C_2)
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
