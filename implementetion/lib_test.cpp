#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#include <eigen3/Eigen/SparseLU>

int main(int argc, char const *argv[])
{
    // Eigen::VectorXd x(9), b(9);
    // Eigen::SparseMatrix < double, Eigen::ColMajor > A;
    // Eigen::SparseLU < Eigen::SparseMatrix < Eigen::EigenSolver::Scalar, Eigen::ColMajor>, Eigen::COLAMDOrdering <Eigen::Index > >   solver;
    // // fill A and b;
    // // Compute the ordering permutation vector from the structural pattern of A
    // solver.analyzePattern(A); 
    // // Compute the numerical factorization 
    // solver.factorize(A); 
    // //Use the factors to solve the linear system 
    // x = solver.solve(b); 

    // Eigen::VectorXd x(9), b(9);
    // Eigen::SparseMatrix < double, Eigen::ColMajor > A;
    Eigen::Matrix2d a;
    a << 2, 0,
         3, 4;
    std::cout << a;


    
    return 0;
}
