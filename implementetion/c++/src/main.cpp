#include <iostream>
#include "headers/EssentMtrx.hpp"
// #include <Eigen/Dense>


int main(int argc, char const *argv[]) {


	EsssentialMatrix Ess;

	// полчуаем теже точки в координатах камеры 2
	// файлы со входными данными - def_cam_(номер камеры)_data, global_coords
	Ess.tranform_features_into_coord_cam();
	// получаем однородные координаты из глобальных для камеры 1 и 2
	//записываем их в файил features

	Ess.get_homogenues_coordinate();
	//
	std::string path = "../input_data/features.txt";
	Ess.simpleRead(path);
	// Ess.printRead();
	// вычисляем E и раскладываем на R и t_x
	Ess.calculateMatrix();




	// double a = 1.5;
	// double b = a;
	// while (a-1) {
	// 	b = a;
	// 	a -= 1;
	// 	a /= 2;
	// 	a += 1;
	// }
	// std::cout << b-1;

	return 0;
}
