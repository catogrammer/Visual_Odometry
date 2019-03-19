#include <iostream>
#include "./src/headers/EssentMtrx.hpp"
// #include <Eigen/Dense>


int main(int argc, char const *argv[]) {
	std::string path = "../input_data/smpldata.txt";
	EsssentialMatrix Ess;
	Ess.simpleRead(path);
	// Ess.printRead();
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
