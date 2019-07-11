this->camera_1 = new Camera();
this->camera_2 = new Camera();

camera_1->read_property_cam("../input_data/def_cam_1_data.txt");
camera_2->read_property_cam("../input_data/def_cam_2_data.txt");

Eigen::MatrixXd orig_feat = read_features("../input_data/global_coords.txt");



std::cout << "*****************features - orig***********************" << '\n';
std::cout << orig_feat << '\n';
std::cout << "****************************************************" << '\n';
std::cout << "cam 1 pose : " << camera_1->cam_pose.transpose() << '\n';
std::cout << "cam 1 horizon : " << camera_1->horizon.transpose() << '\n';
std::cout << "cam 1 normal : " << camera_1->normal.transpose() << '\n';

//
// camera_1->features = coord_in_cam_space(orig_feat.array(), camera_1->cam_pose.array()); //(row,column)
// camera_2->features = coord_in_cam_space(orig_feat.array(), camera_2->cam_pose.array());; //(row,column)

// camera_1->features = orig_feat;
// camera_2->features = orig_feat;

// find transformation matrix

// Eigen::Matrix3d transf_m_1;
// Eigen::Matrix3d transf_m_2;
// transf_m_1 << camera_1->horizon, camera_1->vertical, camera_1->normal;
// transf_m_2 << camera_2->horizon, camera_2->vertical, camera_2->normal;

// transform X_local_1 to X_glob : X_glob = M_1*X_loc + C_1
Eigen::MatrixXd features_in_glob(3, camera_1->features.cols());
for (size_t i = 0; i < (size_t)features_in_glob.cols(); i++) {
    features_in_glob.col(i) << (transf_m_1 * camera_1-> тfeatures.col(i) + camera_1->cam_pose);
}
// std::cout << "Global features : \n" << features_in_glob << '\n';

// transform X_glob to X_local_2 : X_local_2 = M_2^(-1)(X_glob - C_2)
for (size_t i = 0; i < (size_t)camera_1->features.cols(); i++) {
    camera_2->features.col(i) << transf_m_2.inverse() * (features_in_glob.col(i) - camera_2->cam_pose);
}
// std::cout << "*****************features - 2***********************" << '\n';
// std::cout << camera_2.features << '\n';
// std::cout << "****************************************************" << '\n';



// std::cout << "Cam data : "
//           << "\npose : " << camera_1->cam_pose.transpose()
//           << "\nnormal : "   << camera_1->normal.transpose()
//           << "\nhorizon : "  << camera_1->horizon.transpose()
//           << "\nvertical : " << camera_1->vertical.transpose() << '\n';
// std::cout << "local features : \n" << camera_1->features  << '\n';

// ???
// Eigen::Vector3d point_1(0,1,0);
// Eigen::Vector3d point_2(0,5,0);


Eigen::Vector3d point_t(0,1,0);

double D_1 = (-1)*(camera_1->normal.dot(camera_1->cam_pose+point_t));
double D_2 = (-1)*(camera_2->normal.dot(camera_2->cam_pose-point_t));


// double D_1 = (-1)*(camera_1->normal.dot(camera_1->cam_pose));
// double D_2 = (-1)*(camera_2->normal.dot(camera_2->cam_pose));

camera_1->coord_in_cam_space(D_1);
camera_2->coord_in_cam_space(D_2);

std::cout << "*****************features - 1***********************" << '\n';
std::cout << camera_1->features << '\n';
std::cout << "****************************************************" << '\n';

std::cout << "*****************features - 2***********************" << '\n';
std::cout << camera_2->features << '\n';
std::cout << "****************************************************" << '\n';

std::string path = "../input_data/features.txt";
std::ofstream fout(path, std::ofstream::out);
fout << camera_1->features.cols() << '\n';
fout.close();
//
camera_1->write_to_file(path);


Eigen::Vector3d projection_on_palne(Eigen::Vector3d normal, Eigen::Vector3d point, Eigen::Vector3d s, double D){
    return point - (((point.dot(normal)) + D) / (s.dot(normal)))*s;
}
