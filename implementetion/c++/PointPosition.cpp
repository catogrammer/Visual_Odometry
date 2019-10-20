#include "Camera/Camera.hpp"
#include <Eigen/Dense>

Eigen::Vector3d
get_point_position(Eigen::Vector3d point_0_a, Eigen::Vector3d point_1_a,
           Eigen::Vector3d point_0_b, Eigen::Vector3d point_1_b)
{
    Eigen::Vector3d across_point;
    Eigen::Vector3d p_a = point_1_a - point_0_a;
    Eigen::Vector3d p_b = point_1_b - point_0_b;
    double t = (point_0_b[0] - point_0_a[0])/(p_a[0] - p_b[0]);

    across_point << p_a*t + point_0_a;
    return across_point;
}


int main(int argc, char const *argv[])
{
    Camera cam_1;
    Camera cam_2;
    
    cam_1.read_property_cam("../../input_data/point_pos/def_cam_1_data.txt");
    cam_2.read_property_cam("../../input_data/point_pos/def_cam_2_data.txt");

    std::cout << "cam prop:" << cam_1.normal << std::endl;

    Eigen::Vector3d a;
    Eigen::Vector3d b;
    a << 5, 0.5, 0;
    b << 5, -0.5, 0;
    cam_1.features = a;
    cam_2.features = b;
    
    std::cout << "coord : " << cam_1.features.transpose() << std::endl;
    std::cout << "coord : " << cam_2.features.transpose() << std::endl;

    cam_1.transform_featutes_to_global_coord();
    cam_2.transform_featutes_to_global_coord();

    std::cout << "coord : " << cam_1.features.transpose() << std::endl;
    std::cout << "coord : " << cam_2.features.transpose() << std::endl;

    std::cout << "across point : "
              << get_point_position(cam_1.cam_pose, cam_1.features.col(0),
                                    cam_2.cam_pose, cam_2.features.col(0)) 
              << std::endl;

    // Eigen::MatrixXd line_a(2,3);
    // Eigen::MatrixXd line_b(2,3);
    // line_a << 0, 2, 1,
    //           0, 1, 1;
    // line_b << 2, 2, 1,
    //           1, 1, 1;
    
    // std::cout << "line a : " << line_a.row(0) << std::endl << line_a.row(1) << std::endl;
    // std::cout << "across point : "
    //           << get_point_position(line_a.row(0),line_a.row(1),
    //                                 line_b.row(0), line_b.row(1)) 
    //           << std::endl;
    return 0;
}
