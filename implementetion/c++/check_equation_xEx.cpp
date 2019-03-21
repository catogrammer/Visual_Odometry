#include <iostream>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <string>
#include <utility>

bool check_equatation(Eigen::Matrix3d E, Eigen::MatrixXd x, Eigen::MatrixXd _x){
    bool flag = true;
    for (size_t i = 0; i < x.cols(); i++) {
        // bool tmp = !(bool)( x.col(i).transpose()*E*_x.col(i) );
        std::cout << "tmp : " << x.col(i).transpose()*E*_x.col(i) << '\n';
        // flag = flag & !(bool)( x.col(i).transpose()*E*_x.col(i) );
    }
    return flag;
}

Eigen::Matrix3d read_e_matrix(){
    Eigen::Matrix3d essent_matrix;
    std::string path = "../input_data/essential_matrix.txt";
    std::ifstream fin(path);
    if(!fin)
        std::cout << "data doesn't read" << '\n';

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            fin >> essent_matrix(i,j);

    fin.close();
    return essent_matrix;
}

std::pair <Eigen::MatrixXd,Eigen::MatrixXd> read_features() {
    std::string path = "../input_data/coords_for_check_xEx.txt";
    std::ifstream fin(path);
    if(!fin){
        std::cout << "data doesn't read" << '\n';
    }
    size_t count_features = 0;
    fin >> count_features;

    Eigen::MatrixXd features_1(3,count_features);
    Eigen::MatrixXd features_2(3,count_features);
    for (size_t i = 0; i < count_features; i++)
        for (size_t j = 0; j < 3; j++)
            fin >> features_1(j,i);

    for (size_t i = 0; i < count_features; i++)
        for (size_t j = 0; j < 3; j++)
            fin >> features_2(j,i);

    fin.close();
    return std::make_pair(features_1, features_2);
}

int main(int argc, char const *argv[]) {

    Eigen::Matrix3d essent_matrix = read_e_matrix();
    std::cout << essent_matrix << '\n';
    std::pair <Eigen::MatrixXd,Eigen::MatrixXd> features = read_features();

    for (size_t i = 0; i < count_e; i++) {
        if (check_equatation(essent_matrix, features.first, features.second)) std::cout << "equation was executed" << '\n';
        else std::cout << "All bad!!!" << '\n';
    }


    //
    // Eigen::Vector3d _x(1, 32, 1);
    // Eigen::Vector3d x(12, 132, 3);
    // if (check_equatation(essent_matrix, x, _x)) std::cout << "equation was executed" << '\n';
    // else std::cout << "All bad!!!" << '\n';

    return 0;
}
