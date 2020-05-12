#ifndef VISUALIZECLOUDPOINT_HPP
#define VISUALIZECLOUDPOINT_HPP

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/viz/vizcore.hpp"
#include <opencv2/core/softfloat.hpp>

#define DEBUG_LOG_ENABLE

class VisualizeCloudPoint
{
private:
    std::vector<cv::viz::WCloud> cloud_widgets;
    std::vector<std::pair<cv::Mat, cv::Mat>> vec_cloud_points;
    std::vector<cv::Point3f> nav_path;
    std::pair<cv::Matx33d,cv::Matx33d> pair_intrinsic_m;
    cv::viz::Viz3d myWindow;

    void generate_cloud_widgets();
    void generate_nav_path();
    void generate_cam_pose();
public:
    VisualizeCloudPoint(std::vector<std::pair<cv::Mat, cv::Mat>> cp, 
        std::vector<cv::Point3f> nav_p, std::pair<cv::Mat,cv::Mat> K_m_pair): vec_cloud_points(cp),
        nav_path(nav_p), pair_intrinsic_m(K_m_pair) {};
    
    ~VisualizeCloudPoint(){};

    // void generate_truth_path(std::vector<cv::Point3f>,){};

    void show();
};

void
VisualizeCloudPoint::generate_cloud_widgets()
{
    size_t i = 0;
    for (auto &&p_v : vec_cloud_points)
    {

    #ifdef DEBUG_LOG_ENABLE
        std::cout << "Matrix for reshape 1 :\n" << p_v.first << std::endl;
        std::cout << "Matrix for reshape 2 :\n" << p_v.second << std::endl;
    #endif

        if (p_v.first.empty() and p_v.second.empty()){
            std::cout << "Matrix for dtawing is empty!!!" << std::endl;
            return;
        }

        cv::Mat cp_1 = p_v.first.reshape(3, 1);
        cv::Mat cp_2 = p_v.second.reshape(3, 1);

        cp_1.convertTo(cp_1, CV_32FC3);
        cp_2.convertTo(cp_2, CV_32FC3);
        
        cv::viz::WCloud cloud_1_widget(cp_1, cv::viz::Color::white());
        cv::viz::WCloud cloud_2_widget(cp_2, cv::viz::Color::purple());

        cloud_1_widget.setRenderingProperty(cv::viz::POINT_SIZE, 4);
        cloud_2_widget.setRenderingProperty(cv::viz::POINT_SIZE, 4);

        std::string name_widget_1 = "Cloud Widget" + std::to_string(i);
        std::string name_widget_2 = "Cloud Widget" + std::to_string(i+1);
        i += 2;
        this->myWindow.showWidget(name_widget_1, cloud_1_widget);
        this->myWindow.showWidget(name_widget_2, cloud_2_widget);
    }
}

void
VisualizeCloudPoint::generate_nav_path()
{
    cv::Point3f summed = nav_path[0];
    for (size_t i = 0; i < nav_path.size() - 1; i++)
    {
        cv::viz::WLine line_widget(summed, summed+nav_path[i+1],
            cv::viz::Color::white());
    #ifdef DEBUG_LOG_ENABLE
        std::cout << "Coords :\n" << summed << "-" << summed+nav_path[i+1] 
        << ":" << nav_path[i] << std::endl << &line_widget << std::endl;
    #endif
        summed += nav_path[i+1];

        std::string name_widget = "Line Widget" + std::to_string(i);
        this->myWindow.showWidget(name_widget, line_widget);
    }
}

// void
// VisualizeCloudPoint::generate_truth_path()
// {

// }

void
VisualizeCloudPoint::generate_cam_pose()
{
    cv::Point3f summed = nav_path[0];
    cv::Matx33d f_intr_m = pair_intrinsic_m.first;
    cv::Matx33d s_intr_m = pair_intrinsic_m.second;

    for (size_t i = 0; i < nav_path.size(); i++)
    {
        cv::viz::WCameraPosition cam_wp(f_intr_m, 1, cv::viz::Color::white());

        Vec3f cam_pos(summed), cam_focal_point(0.0f,0.0f,0.0f), cam_y_dir(-1.0f,0.0f,0.0f);
        // cam pose / direction view cam / dir by y/x/z
        Affine3f cam_pose = viz::makeCameraPose(cam_pos, cam_focal_point, cam_y_dir);
        summed += nav_path[i+1];

    #ifdef DEBUG_LOG_ENABLE
        std::cout << "Size path : " << nav_path.size() << std::endl << "i = "
        << i <<  std::endl << "Intrinsic matrix :\n" << f_intr_m 
        << std::endl;
    #endif

        std::string cam_widget = "Cam " + std::to_string(i) + " Widget";
        this->myWindow.showWidget(cam_widget, cam_wp, cam_pose);
    }
}

void
VisualizeCloudPoint::show()
{
    this->myWindow = cv::viz::Viz3d("Viz Demo");
    this->myWindow.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());

    // generate_cloud_widgets();
    generate_cam_pose();
    generate_nav_path();

    while(!this->myWindow.wasStopped())
    {
        this->myWindow.spinOnce(1, true);
    }
}

#endif