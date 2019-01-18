#include <iostream>
#include "./src/headers/EssentMtrx.hpp"
// #include <Eigen/Dense>


int main(int argc, char const *argv[]) {
	std::string path = "./src/input_data/smpldata.txt";
	EsssentialMatrix Ess;
	Ess.simpleRead(path);
	// Ess.printRead();
	Ess.calculateMatrix();


	return 0;
}
