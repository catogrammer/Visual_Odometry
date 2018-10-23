#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
// #include <tr1>


/*
	xEx' = 0
*/

struct Coordinate{
	int coord[3] = {0, 0, 0};
	int coordT[3][1] = {{0}, {0}, {0}};

	Coordinate(int x, int y, int z){
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
		std::cout << "[" << coord[0] << coord[1] << coord[2] << "]";
	}
	void printCoordT() {
		for (size_t i = 0; i < 3; i++)
			std::cout << "[" << coordT[0][i] <<"]" << '\n';
	}
};

struct Polynom{
	int *polynom;
	size_t curr = 0;
	size_t len = 0;

	Polynom(size_t len){
		this->len = len;
		this->polynom = new int[len];
	}

	void add(int arg) {
		if(len > 0 && curr < len){
			this->polynom[curr] = arg;
			curr++;
		}else
			std::cout << "Length isn't right!" << '\n';
	}

	void addP(Polynom p) {
		if(this->len < p.len){
			std::cout << "You can't add polynom more size than size current polynom" << '\n';
		}else{
			for (size_t i = 0; i < p.len; i++){
				this->polynom[this->curr] = p.polynom[i];
				this->curr++;
			}
		}
	}

	Polynom(const Polynom &p) : len(p.len), polynom(new int[p.len]) {
		std::copy(p.polynom, p.polynom + this->len, this->polynom);
	}

	void multiple(int factor) {
		for (size_t i = 0; i < len; i++)
			this->polynom[i] *= factor;
	}

	void printPolynom() {
		for (size_t i = 0; i < len; i++)
			std::cout << "[ " << this->polynom[i] << " ]";
		std::cout << '\n';
	}

	~Polynom(){
		if(polynom != nullptr)
			delete[] polynom;
	}

};

struct EsssentialMatrix{
	std::vector< std::pair<Coordinate, Coordinate> > features;
	int **EssentMtrx;

	Polynom calcEquation(int x[3], int _x[3]) {
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


	void calcMatrix(/* arguments */) {
		for (auto el : features) {
			Polynom p = calcEquation(el.first.coord, el.second.coord);
			p.printPolynom();
		}
	}

	void simpleRead() {
		std::ifstream fin("smpldata.txt");
		std::vector<Coordinate> tmp1;
		std::vector<Coordinate> tmp2;
		int x, y, z;
		for (size_t i = 0; i < 9; i++) {
			fin >> x;
			fin >> y;
			fin >> z;
			Coordinate c1 = Coordinate(x,y,z);
			tmp1.push_back(c1);
		}
		for (size_t i = 0; i < 9; i++) {
			fin >> x;
			fin >> y;
			fin >> z;
			Coordinate c2 = Coordinate(x,y,z);
			tmp2.push_back(c2);
		}
		for (size_t i = 0; i < 9; i++)
			features.push_back(std::make_pair(tmp1[i], tmp2[i]));

		fin.close();
	}

	void printRead() {
		for (size_t i = 0; i < 9; i++) {
			features[i].first.printCoord();
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

int main(int argc, char const *argv[]) {
	EsssentialMatrix Ess;
	Ess.simpleRead();
	Ess.calcMatrix();

	return 0;
}
