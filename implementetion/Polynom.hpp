#ifndef POLYNOM_HPP
#define POLYNOM_HPP

class Polynom {
	double *polynom;
	size_t curr = 0;
	size_t len = 0;
    
public:
	Polynom(size_t len){
		this->len = len;
		this->polynom = new double[len];
	}

	double       *get_polynom()       {return polynom; }
    const double *get_polynom() const {return polynom; }

	void add(double arg) {
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

	Polynom(const Polynom &p) : len(p.len), polynom(new double[p.len]) {
		std::copy(p.polynom, p.polynom + this->len, this->polynom);
	}

	void multiple(double factor) {
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