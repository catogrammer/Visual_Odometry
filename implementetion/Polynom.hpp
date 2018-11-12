#ifndef POLYNOM_HPP
#define POLYNOM_HPP

class Polynom {
	int *polynom;
	size_t curr = 0;
	size_t len = 0;
    
public:
	Polynom(size_t len){
		this->len = len;
		this->polynom = new int[len];
	}

	int       *get_polynom()       {return polynom; }
    const int *get_polynom() const {return polynom; }

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

#endif //POLYNOM