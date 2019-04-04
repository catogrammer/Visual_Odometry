#include <eigen3/Eigen/SparseLU> //метод факторизации (LU - разложение),
// где L-нижняя треугольная U-верхняя треугольная матрица [одна из разновидностей метода Гаусса]
#include <eigen3/Eigen/Dense>
#include <iostream>

int main(int argc, char const *argv[]) {
    Eigen::Matrix3d EssentMtrx;
    EssentMtrx << 0, 0.61, 0,
                  0, 0, -0.61,
                  0,  1,   0;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(EssentMtrx, Eigen::ComputeThinU | Eigen::ComputeThinV);
    std::cout << "singular values are:" << std::endl << svd.singularValues() << std::endl;
    std::cout << "U matrix: \n" << svd.matrixU() << std::endl;
    std::cout << "V matrix:" << std::endl << svd.matrixV() << std::endl;

    return 0;
}
