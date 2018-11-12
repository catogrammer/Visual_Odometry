#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD> 



int main(int argc, char const *argv[])
{
    Eigen::MatrixXd A(3,3);
    Eigen::Vector3d b;
    A << 1,2,3,  4,5,6,  7,8,10;
    b << 3, 3, 4;
    std::cout << "Here is the matrix A:\n" << A << std::endl;
    std::cout << "Here is the vector b:\n" << b << std::endl;
    Eigen::Vector3d x = A.colPivHouseholderQr().solve(b);
    std::cout << "The solution is:\n" << x << std::endl;
    
    return 0;
}
