#include <iostream>
#include "EssentMtrx.hpp"
#include <Eigen/Dense>


int main(int argc, char const *argv[]) {

	EsssentialMatrix Ess;
	Ess.simpleRead();
	Ess.calcMatrix();


	return 0;
}
