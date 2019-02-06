#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>

int main(int argc, char const *argv[])
{
    double *features = new double[9];
    for (size_t i = 0; i < 9; i++) {
        features[i] = i;
    }


    delete[] features;

    for (size_t i = 0; i < 9; i++) {
        std::cout << features[i] << '\n';
    }
    // Eigen::Matrix3d EssentMtrx = Eigen::Matrix3d::Ones();
    // Eigen::MatrixXd A(9,9);
    // A << 0.4, 2, 3, 0, 2, 3, 1, 2, 1,
    //      4, 5, 6, 1, 2, 3, 1, 2, 3,
    //      4, 5, 6, 1, 2, 3, 1, 2, 3,
    //      4, 5, 6, 7, 2, 3, 1, 2, 4.6,
    //      1, 8, 3, 1, 6.5, 3, 6, 9, 3,
    //      4, 5, 6, 0, 2, 9, 1, 2, 3,
    //      1, 8.1, 3, 1, 2, 3, 5, 2, 0,
    //      4, 5, 2, 9, 2, 3, 1, 2, 3,
    //      5, 7, 9, 1, 2, 6, 0.2, 5, 8;



    // std::cout << "Here is the matrix A:\n" << A << std::endl;
    // Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A);
    // std::cout << "The rank of A is " << lu_decomp.rank() << std::endl;
    // Eigen::MatrixXd x = lu_decomp.kernel();
    // std::cout << "Here is a matrix whose columns form a basis of the null-space of A:\n"
    //     << x << std::endl;

    // for(size_t i = 0, k = 0; i < 3; i++) {
    //     for(size_t j = 0; j < 3; j++, k++) {
    //         EssentMtrx(i, j) = x.col(0)(k);
    //     }
    // }
    // std::cout << std::endl << EssentMtrx << std::endl;
    //
    // Eigen::JacobiSVD<Eigen::MatrixXd> svd(EssentMtrx, Eigen::ComputeThinU | Eigen::ComputeThinV);
    //
    // std::cout << "Its singular values are:" << std::endl << svd.singularValues() << std::endl;
    // std::cout << "Its left singular vectors are the columns of the thin U matrix:" << std::endl << svd.matrixU() << std::endl;
    // std::cout << "Its right singular vectors are the columns of the thin V matrix:" << std::endl << svd.matrixV() << std::endl;

    return 0;
}
