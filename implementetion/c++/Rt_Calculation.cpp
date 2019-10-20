#include <iostream>
#include "EssentialMatrix/EssentialMatrix.hpp"

int main(int argc, char const *argv[]) {

	EsssentialMatrix Ess;

	/* полчуаем теже точки в координатах камеры 2 
	 * файлы со входными данными - def_cam_(номер камеры)_data, global_coords
	 */

	Camera* camera_1 = new Camera();
	Camera* camera_2 = new Camera();

	camera_1->read_property_cam("../../input_data/def_cam_1_data.txt");
	camera_2->read_property_cam("../../input_data/def_cam_2_data.txt");
	Eigen::MatrixXd orig_features = Ess.read_features("../../input_data/global_coords.txt");

	Ess.tranform_features_into_coord_cam(orig_features, camera_1, camera_2);
	/* получаем однородные координаты из глобальных для камеры 1 и 2
	 * записываем их в файил features
	 */

	Ess.get_homogenues_coordinate();
	Ess.simpleRead("../../input_data/features.txt");
	// Ess.printRead();

	// вычисляем E и раскладываем на R и t_x
	
	Ess.calculate_from_svd();

	// Ess.calculate_from_ker();




	return 0;
}
