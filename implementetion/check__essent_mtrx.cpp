#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <utility>


struct data{
  double* normal;
  double* pose; //coordinate of position
  double* deep;
  std::vector<std::pair<double, double> >* features;
  double* offset_pose;
  double* offset_nomal;
};

void printArr(double* arr, size_t len) {
  for (size_t i = 0; i < len; i++)
    std::cout << "[ " << arr[i] << " ]";
  std::cout << '\n';
}

double* generate_data(int left_range, int right_range){
  double* data = new double[3];
  for (size_t i = 0; i < 3; i++) {
    data[i] = std::rand() % left_range - right_range;
  }
  return data;
}

std::vector<std::pair<double, double> >* create_features(int left_range, int right_range, size_t size){
  std::vector<std::pair<double, double> >* features = new std::vector<std::pair<double, double> >(size);
  for(auto &el : *features){
    double x = std::rand() % left_range - right_range;
    double y = std::rand() % left_range - right_range;
    el = std::make_pair(x, y);
  }
  return features;
}

void create_data(data* base_d){
  //---------------------may be randome -----------------/
  base_d->normal = generate_data(20, 10);
  base_d->pose = generate_data(20, 10);
  base_d->deep = generate_data(10, 0);
  base_d->features = create_features(20, 10, 9);
  //--------------------------------------------------------/
  //-----------------select myself--------------------------/
  base_d->offset_pose = new double[3] {7, 5, 1};
  base_d->offset_nomal = new double[3] {5, 5, 0};
}

void scale_by(double* arr, size_t len, double scale_coeff) {
  for (size_t i = 0; i < len; i++) {
    arr[i] *=scale_coeff;
  }
}

void scaling_coordinate_to_global_measur_sys(data* base_d, double scale_coeff) {
  // scale_by(base_d->normal, 3, scale_coeff); //?? нужно ли масштаб нормаль и  положение?
  // scale_by(base_d->pose, 3, scale_coeff);
  scale_by(base_d->deep, 3, scale_coeff);
  for (auto &el : *base_d->features) {
    el.first *= scale_coeff;
    el.second *= scale_coeff;
  }
}

void printData(data* base_d){
  std::cout << "normal: ";
  printArr(base_d->normal, 3);
  std::cout << "position of cam: ";
  printArr(base_d->pose, 3);
  std::cout << "deep of point: ";
  printArr(base_d->deep, 3);
  for ( auto el : *base_d->features ) {
    std::cout << '[' << el.first << ',' << el.second << ']' << '\n';
  }
  std::cout << "offset pose: ";
  printArr(base_d->offset_pose, 3);
  std::cout << "offset normal: ";
  printArr(base_d->offset_nomal, 3);
}

int main(int argc, char const *argv[]) {
  // std::srand(std::time(0)); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  data* base_d = new data;
  create_data(base_d);
  printData(base_d);
  double scale_coeff = 0.5;
  //need scale  to global coord only normal, features, pose, deep
  scaling_coordinate_to_global_measur_sys(base_d, scale_coeff);
  printData(base_d);



  // delete[] normal;
  return 0;
}
