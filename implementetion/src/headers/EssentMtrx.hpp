#ifndef ESSENTMTRX_HPP
#define ESSENTMTRX_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>

#include <eigen3/Eigen/SparseLU> //метод факторизации (LU - разложение),
// где L-нижняя треугольная U-верхняя треугольная матрица [одна из разновидностей метода Гаусса]
#include <eigen3/Eigen/Dense>

#include "Polynom.hpp"
#include "AdapterMyMtrxToEigenMtrx.hpp"

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
	std::vector<Polynom> system_lin_equat;
	// int EssentMtrx[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	Eigen::Matrix3d EssentMtrx = Eigen::Matrix3d::Ones();

public:

	Polynom calculateEquation(double x[3], double _x[3]) {
		Polynom tmp[3] = {Polynom(3), Polynom(3), Polynom(3)};
		Polynom result = Polynom(9);
		for (size_t i = 0; i < 3; i++) {
			tmp[i].add(x[0]);
			tmp[i].add(x[1]);
			tmp[i].add(x[2]);
		}
		for (size_t i = 0; i < 3; i++) {
			tmp[i].multiple(_x[i]);
			result.addP(tmp[i]);
		}
		return result;
	}

	void calculateMatrix(/* arguments */) {
		for (auto el : features) {
			Polynom p = calculateEquation(el.first.coord, el.second.coord);
			system_lin_equat.push_back(p);
		}

		AdapterMyMtrxToEigenMtrx calcMatrix(system_lin_equat); /* make static function,
		 that don't create exemplar of class ???! */
		// calcMatrix = AdapterMyMtrxToEigenMtrx::getEigenMtrx(system_lin_equat);
		Eigen::MatrixXd A = calcMatrix.getEigenMtrx();
		std::cout << A << std::endl;
		Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A);

		Eigen::MatrixXd x = lu_decomp.kernel();
		std::cout << "plurality of vectors of kernel:\n" << lu_decomp.kernel() << std::endl;
		std::cout << "I choose vector from first column" << std::endl;
		std::cout << "Col is:\n" << x.col(0).transpose() << std::endl;
		for(size_t i = 0, k = 0; i < 3; i++) {
			for(size_t j = 0; j < 3; j++, k++) {
				EssentMtrx(i, j) = x.col(0)(k);
			}
		}
		std::cout << "The solution is:\n" << EssentMtrx << std::endl;
		Eigen::JacobiSVD<Eigen::MatrixXd> svd(EssentMtrx, Eigen::ComputeThinU | Eigen::ComputeThinV);

		std::cout << "Its singular values are:" << std::endl << svd.singularValues() << std::endl;
		std::cout << "Its left singular vectors are the columns of the thin U matrix:" << std::endl << svd.matrixU() << std::endl;
		std::cout << "Its right singular vectors are the columns of the thin V matrix:" << std::endl << svd.matrixV() << std::endl;
	}

	void simpleRead(std::string path) {
		std::ifstream fin(path);
		if(!fin){
			std::cout << "data doesn't read" << '\n';
		}
		std::vector<Coordinate> tmp1;
		std::vector<Coordinate> tmp2;
		double x, y, z;
		for (size_t i = 0; i < 9; i++) {
			fin >> x;
			fin >> y;
			fin >> z;
			// Coordinate c1 = Coordinate(x,y,z);
			tmp1.push_back( Coordinate(x,y,z) );
		}
		for (size_t i = 0; i < 9; i++) {
			fin >> x;
			fin >> y;
			fin >> z;
			// Coordinate c2 = Coordinate(x,y,z);
			tmp2.push_back( Coordinate(x,y,z) );
		}
		for (size_t i = 0; i < 9; i++)
			features.push_back(std::make_pair(tmp1[i], tmp2[i]));

		fin.close();
	}

	void printRead() {
		for (size_t i = 0; i < 9; i++) {
			features[i].first.printCoord();
			std::cout << '\t';
			features[i].second.printCoord();
			std::cout << '\n';
		}
	}

	// int readData(/* arguments */) {
	// 	/*
	// 		image_1
	// 		cn: x y z
	// 		image_2
	// 		cn: x y z
	// 	*/
	// 	char buff[50];
	// 	auto m = cmatch {};
	// 	std::ifstream fin("data.txt");
	// 	fin.getline(buff, 50);
	//
	// 	if (reg_match("image_1", m, regex{ R"(\w+)"} )) {
	// 		std::cout << "Ohhh eahh" << '\n';
	// 	} else if (reg_match("image_2", m, regex{ R"(\w+)"} )) {
	// 		std::cout << "Fuck" << '\n';
	// 	}else{
	// 		std::cout << "Error input data" << '\n';
	// 	}
	// 	fin.close();
	// }
};

#endif //EssentMtrx