#include <iostream>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <string>
#include <utility>

bool check_equatation(Eigen::Matrix3d E, Eigen::MatrixXd x, Eigen::MatrixXd _x){
    bool flag = true;
    for (size_t i = 0; i < x.cols(); i++) {
        std::cout << "tmp : " << x.col(i).transpose()*E*_x.col(i) << '\n';
        flag = flag & ( x.col(i).transpose()*E*_x.col(i) < 1e-10);
    }
    return flag;
}

std::vector<Eigen::Matrix3d> read_e_matrix(){

    std::string path = "../input_data/essential_matrix.txt";
    std::ifstream fin(path);
    if(!fin)
        std::cout << "data doesn't read" << '\n';
    size_t count_e = 0;
    fin >> count_e;
    std::vector<Eigen::Matrix3d> essent_matrix(count_e);
    for (auto &mtr : essent_matrix) {
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                fin >> mtr(i,j);
    }

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

    std::vector<Eigen::Matrix3d> essent_matrix = read_e_matrix();
    for (auto el : essent_matrix) {
        std::cout << "E : \n" << el << '\n';
    }
    std::pair <Eigen::MatrixXd,Eigen::MatrixXd> features = read_features();
    std::cout << "first : \n" << features.first << '\n' << "second : \n"
    << features.second << '\n';

    for (auto mtrx : essent_matrix) {
        if (check_equatation(mtrx, features.first, features.second)) std::cout << "equation was executed" << '\n';
        else std::cout << "All bad!!!" << '\n';
    }

    //
    // Eigen::Vector3d _x(0, 2, 3);
    // Eigen::Vector3d x(1.94975, 2, 2.94975);
    // if ( check_equatation(essent_matrix[0], x, _x) ) std::cout << "equation was executed" << '\n';
    // else std::cout << "All bad!!!" << '\n';

    return 0;
}
