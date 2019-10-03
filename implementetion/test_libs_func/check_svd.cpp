#include <eigen3/Eigen/SparseLU> //метод факторизации (LU - разложение),
// где L-нижняя треугольная U-верхняя треугольная матрица [одна из разновидностей метода Гаусса]
#include <eigen3/Eigen/Dense>
#include <iostream>

int main(int argc, char const *argv[]) {
    // Eigen::Matrix3d EssentMtrx;
    // EssentMtrx << 0, 0.61, 0,
    //               0, 0, -0.61,
    //               0,  1,   0;
    // Eigen::JacobiSVD<Eigen::MatrixXd> svd(EssentMtrx, Eigen::ComputeThinU | Eigen::ComputeThinV);
    // std::cout << "singular values are:" << std::endl << svd.singularValues() << std::endl;
    // std::cout << "U matrix: \n" << svd.matrixU() << std::endl;
    // std::cout << "V matrix:" << std::endl << svd.matrixV() << std::endl;

    Eigen::MatrixXf m(2,2);
    Eigen::MatrixXf n(2,2);
    Eigen::MatrixXf result(2,2);
    Eigen::MatrixXf a(2,1);
    Eigen::VectorXf b(2);
    a << 1,1;
    b << 1,1;
         // 1,1;
    m << 1,2,
         3,4;
    n << 5,6,
         7,8;
    result = m * n;
    std::cout << "-- Matrix m*n: --" << std::endl << result << std::endl << std::endl;
    result = m.array() * n.array();
    std::cout << "-- Array m*n: --" << std::endl << result << std::endl << std::endl;
    result = m.cwiseProduct(n);
    std::cout << "-- With cwiseProduct: --" << std::endl << result << std::endl << std::endl;
    result = a + b;
    std::cout << "-- Array m + 4: --" << std::endl << result << std::endl << std::endl;


    return 0;
}
