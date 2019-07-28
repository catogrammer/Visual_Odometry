#ifndef ESSENTMTRX_HPP
#define ESSENTMTRX_HPP

#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <utility>
#include <string>

#include <eigen3/Eigen/SparseLU> //метод факторизации (LU - разложение),
// где L-нижняя треугольная U-верхняя треугольная матрица [одна из разновидностей метода Гаусса]
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>
#include <eigen3/Eigen/Eigenvalues>


#include "Camera.hpp"

/*
	xEx' = 0
*/

struct Coordinate {
	double coord[3] = {0, 0, 0};
	double coordT[3][1] = {{0}, {0}, {0}};

	Coordinate(double x, double y, double z){
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	}

	void Transposition() {
		coordT[0][0] = coord[0];
		coordT[1][0] = coord[1];
		coordT[2][0] = coord[2];
	}

	void printCoord() {
		std::cout << "[" << coord[0] << ' ' << coord[1] << ' ' << coord[2] << "]";
	}
	void printCoordT() {
			std::cout << "[" << coordT[0][0] << ' ' << coordT[0][1] << ' ' << coordT[0][2] <<"]" << '\n';
	}
};

class EsssentialMatrix {
	std::vector< std::pair<Coordinate, Coordinate> > features;
	std::vector<double*> system_lin_equat;
	Eigen::Matrix3d EssentMtrx = Eigen::Matrix3d::Ones();
	Camera* camera_1;
	Camera* camera_2;

	Eigen::VectorXd SVD_decompose();
	void calc_UV_S__R_t_x();
	Eigen::MatrixXd get_Ae_matrix();
	Eigen::MatrixXd get_kernel_Ae_matrix();

public:
	Eigen::MatrixXd read_features(std::string path);

	void tranform_features_into_coord_cam();
	void get_homogenues_coordinate();

	void calculate_from_svd();
	void calculate_from_ker();

	void simpleRead(std::string path);

	void printRead();



	void calculate_Ess_matrix(Eigen::VectorXd v);
};

#endif //EssentMtrx
