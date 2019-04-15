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

public:

	void tranform_features_into_coord_2_cam() {
	    this->camera_1 = new Camera();
		this->camera_2 = new Camera();
	    camera_1->read_property_cam("../input_data/def_cam_1_data.txt");
		camera_2->read_property_cam("../input_data/def_cam_2_data.txt");

		// normalisation of coordinates
		camera_1->normal    = camera_1->normal.normalized(); // y
		camera_1->horizon   = camera_1->horizon.normalized(); // x
		camera_2->normal    = camera_2->normal.normalized(); //loc in cam pose coord
		camera_2->horizon   = camera_2->horizon.normalized(); //loc in cam pose coord

		// camera_1.features = read_local_coord("../input_data/homogen_features.txt");
		camera_1->read_features("../input_data/global_coords.txt");
		camera_2->features = Eigen::MatrixXd(3, camera_1->features.cols()); //(row,column)

		// std::cout << "*****************features - 1***********************" << '\n';
		// std::cout << camera_1.features << '\n';
		// std::cout << "****************************************************" << '\n';

		// find y'
		camera_1->vertical << camera_1->horizon.cross(camera_1->normal); // == x * n
		camera_2->vertical << camera_2->horizon.cross(camera_2->normal);

		// find transformation matrix
		Eigen::Matrix3d transf_m_1;
		Eigen::Matrix3d transf_m_2;
		transf_m_1 << camera_1->horizon, camera_1->vertical, camera_1->normal;
		transf_m_2 << camera_2->horizon, camera_2->vertical, camera_2->normal;

		// transform X_local_1 to X_glob : X_glob = M_1*X_loc + C_1
		Eigen::MatrixXd features_in_glob(3, camera_1->features.cols());
		for (size_t i = 0; i < (size_t)features_in_glob.cols(); i++) {
			features_in_glob.col(i) << (transf_m_1 * camera_1->features.col(i) + camera_1->cam_pose);
		}
		// std::cout << "Global features : \n" << features_in_glob << '\n';

		// transform X_glob to X_local_2 : X_local_2 = M_2^(-1)(X_glob - C_2)
		for (size_t i = 0; i < (size_t)camera_1->features.cols(); i++) {
			camera_2->features.col(i) << transf_m_2.inverse() * (features_in_glob.col(i) - camera_2->cam_pose);
		}
		// std::cout << "*****************features - 2***********************" << '\n';
		// std::cout << camera_2.features << '\n';
		// std::cout << "****************************************************" << '\n';
	}

	void get_homogenues_coordinate() {
	    double D_1 = (-1)*(camera_1->normal.dot(camera_1->cam_pose));
		camera_1->transform_to_homogen(D_1);

	    // std::cout << "global features : \n" << camera_1->features << '\n';
	    // std::cout << "Cam data : "
	    //           << "\npose : " << camera_1->cam_pose.transpose()
	    //           << "\nnormal : "   << camera_1->normal.transpose()
	    //           << "\nhorizon : "  << camera_1->horizon.transpose()
	    //           << "\nvertical : " << camera_1->vertical.transpose() << '\n';
	    // std::cout << "local features : \n" << camera_1->features  << '\n';

	    double D_2 = (-1)*(camera_2->normal.dot(camera_2->cam_pose));
		camera_2->transform_to_homogen(D_2);

		std::string path = "../input_data/features.txt";
		std::ofstream fout(path, std::ofstream::out);
		fout << camera_1->features.cols() << '\n';
		fout.close();
		//
		camera_1->write_to_file(path);
		camera_2->write_to_file(path);

	}

	double* calc_polynom(double x[3], double _x[3]) {
		int count = 0;
		double *equation = new double[9];
		for (size_t i = 0; i < 3; i++) {
			for (size_t j = 0; j < 3; j++) {
				equation[count++] = x[j]*_x[i];
			}
		}
		return equation;
	}

	void calc_UV_S__R_t_x() {
		Eigen::JacobiSVD<Eigen::MatrixXd> svd(EssentMtrx, Eigen::ComputeThinU | Eigen::ComputeThinV);
		std::cout << "singular values are:" << std::endl << svd.singularValues() << std::endl;
		std::cout << "U matrix: \n" << svd.matrixU() << std::endl;
		std::cout << "V matrix:" << std::endl << svd.matrixV() << std::endl;
		/*
			[t]_x = U * W * S * U^T   S - singular values
			R = U * W^-1 * V^T
			diag(S) = [s, s, 0]
			W^-1 = W^T=[[0, 1, 0], [-1, 0, 0], [0, 0, 1]]
		*/

		Eigen::Matrix3d W_T, W, R, S, t_x;
		W_T << 0, 1, 0,
			  -1,  0, 0,
			   0,  0, 1;
		W <<   0,  -1, 0,
			   1,  0, 0,
			   0,  0, 1;
		S << svd.singularValues()[0], 0, 0,
			 0, svd.singularValues()[1], 0,
			 0, 					  0, 0;
		// std::cout << "Matrix W is transpose : \n" << W_T.transpose() << '\n';
		R = svd.matrixU() * W_T * svd.matrixV().transpose();
		std::cout << "Rotation matrix : \n" << R << '\n';
		t_x = svd.matrixU() * W * svd.matrixU().transpose();
		std::cout << "Matrix t_x : \n" << t_x << '\n';

		/*
			t_k = [[0, -t_z, t_y], [t_z, 0, -t_x], [-t_y, t_x, 0]]
		*/

		std::cout << "Vector t_x : \n" << t_x(2,1) << ' '
		 		  << t_x(2,0) << ' ' << t_x(1,0) << '\n';
	}

	void calculateMatrix(/* arguments */) {
		std::cout << "features size : " << features.size() << '\n';

		for (auto el : features) {
			double *p = calc_polynom(el.first.coord, el.second.coord);
			system_lin_equat.push_back(p);
		}

		size_t size_m = features.size();
		Eigen::MatrixXd A(size_m, 9);
		for (size_t i = 0; i < size_m; i++) {
			for (size_t j = 0; j < 9; j++) {
				A(i,j) = system_lin_equat[i][j];
			}
		}
		std::cout << "Matrix A : \n" << A << '\n';
		Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A);

		Eigen::MatrixXd x = lu_decomp.kernel();
		std::cout << "plurality vectors of kernel:\n" << lu_decomp.kernel() << std::endl;

		std::cout << "!!!!!!!!!!!!!!checking!!!!!!!!!!!!!!!" << '\n';
		std::cout << "A*x(0) = " << (A*x.col(0)).transpose() << '\n';
		std::cout << "A*x(1) = " << (A*x.col(1)).transpose() << '\n';
		std::cout << "A*x(2) = " << (A*x.col(2)).transpose() << '\n';
		std::cout << "!!!!!!!!!!!!!!checking!!!!!!!!!!!!!!!" << '\n';

		std::string path= "../input_data/essential_matrix.txt";
		std::ofstream fout(path);
		if(!fout){
			std::cout << "file dosen't open" << '\n';
		}

		// для каждого вектора из ядра составляем матрицу Е
		// и вычисляем R и t_x из разожения SVD в calc_UV_S__R_t_x
		fout << x.cols() << '\n'; // number of vectors kernel
		for (size_t col_ker = 0; col_ker < (size_t)x.cols(); col_ker++) {
			std::cout << "*******-" << col_ker << "-*****************" << '\n';
			for(size_t i = 0, k = 0; i < 3; i++) {
				for(size_t j = 0; j < 3; j++, k++) {
					EssentMtrx(j, i) = x.col(col_ker)(k);
				}
			}

			fout << EssentMtrx << "\n\n";
			std::cout << "The solution is:\n" << EssentMtrx << std::endl;
			calc_UV_S__R_t_x();

			// std::cout << A*x.col(0) << '\n'
		}
		fout.close();

	}

	void simpleRead(std::string path) {
		std::ifstream fin(path);
		if(!fin){
			std::cout << "data doesn't read" << '\n';
		}
		std::vector<Coordinate> tmp1;
		std::vector<Coordinate> tmp2;
		double x, y, z;
		int size = 0;
		fin >> size;
		for (size_t i = 0; i < (size_t)size; i++) {
			fin >> x;
			fin >> y;
			fin >> z;
			// Coordinate c1 = Coordinate(x,y,z);
			tmp1.push_back( Coordinate(x,y,z) );
		}
		for (size_t i = 0; i < (size_t)size; i++) {
			fin >> x;
			fin >> y;
			fin >> z;
			// Coordinate c2 = Coordinate(x,y,z);
			tmp2.push_back( Coordinate(x,y,z) );
		}
		for (size_t i = 0; i < (size_t)size; i++)
			features.push_back(std::make_pair(tmp1[i], tmp2[i]));

		printRead();
		fin.close();
	}

	void printRead() {
		for (size_t i = 0; i < features.size(); i++) {
			features[i].first.printCoord();
			std::cout << '\t';
			features[i].second.printCoord();
			std::cout << '\n';
		}
	}
};

#endif //EssentMtrx
