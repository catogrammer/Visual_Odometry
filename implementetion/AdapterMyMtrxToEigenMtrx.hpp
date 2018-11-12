#ifndef ADAPTERMYMTRXTOEIGENMTRX_HPP
#define ADAPTERMYMTRXTOEIGENMTRX_HPP

#include <eigen3/Eigen/Dense>
#include <vector>
#include "Polynom.hpp"


class AdapterMyMtrxToEigenMtrx {
    Eigen::MatrixXd A;

public:
    AdapterMyMtrxToEigenMtrx(std::vector<Polynom> vec_of_p, int row = 9, int column = 9){
            A = Eigen::MatrixXd(row, column);
            for(size_t i = 0; i < row; i++) {
                for(size_t j = 0; j < column; j++){
                    A(i,j) = vec_of_p[i].get_polynom()[j];
                }
            }
    };

    Eigen::MatrixXd getEigenMtrx(){
        return A;
    }

};

#endif //AdapterMyMtrxToEigenMtrx