#ifndef VISUALIZECLOUDPOINT_HPP
#define VISUALIZECLOUDPOINT_HPP

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/viz/vizcore.hpp"
#include <opencv2/core/softfloat.hpp>

// #define DEBUG_LOG_ENABLE

class VisualizeCloudPoint
{
private:
    cv::viz::Viz3d myWindow;
    std::vector<std::pair<cv::Mat, cv::Mat>> vec_cloud_points;
    std::vector<cv::viz::WCloud> cloud_widgets;

    size_t w_counter = 0;

    void generate_cloud_widgets();
    // void generate_nav_path();
    void generate_cam_pose();
public:
    VisualizeCloudPoint(std::vector<std::pair<cv::Mat, cv::Mat>> cp):
        vec_cloud_points(cp)
    {
        this->myWindow = cv::viz::Viz3d("Viz Demo");
        this->myWindow.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());
    };
    
    ~VisualizeCloudPoint(){};

    // void generate_truth_path(std::vector<cv::Point3f>,){};
    void show_path(std::vector<cv::Point3f> path,
        std::pair<cv::Matx33d,cv::Matx33d> pair_intrinsic_m,
        cv::viz::Color color, bool show_cam);

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
VisualizeCloudPoint::show_path(std::vector<cv::Point3f> path,
    std::pair<cv::Matx33d,cv::Matx33d> pair_intrinsic_m,
    cv::viz::Color color = cv::viz::Color::white(), bool show_cam = false)
{
    cv::Point3f summed = path[0];
    cv::Matx33d f_intr_m = pair_intrinsic_m.first;
    cv::Matx33d s_intr_m = pair_intrinsic_m.second;

    for (size_t i = 0; i < path.size(); i++)
    {
        cv::viz::WLine line_widget(summed, summed + path[i+1], color);

        if (show_cam)
        {
            cv::viz::WCameraPosition cam_wp(f_intr_m, 0.7, color);
            Vec3f cam_pos(summed);                  // cam pose
            Vec3f cam_focal_point(0.0f,0.0f,0.0f);  // direction view cam
            Vec3f cam_y_dir(1.0f,0.0f,0.0f);       // dir by y/x/z
            
            Affine3f cam_pose = viz::makeCameraPose(cam_pos, cam_focal_point, cam_y_dir);
            std::string name_cam_widget = "Cam " + std::to_string(w_counter) + " Widget";
            this->myWindow.showWidget(name_cam_widget, cam_wp, cam_pose);
        }

        summed += path[i+1];
        std::string name_l_widget = "Line Widget" + std::to_string(w_counter);
        w_counter++;
        this->myWindow.showWidget(name_l_widget, line_widget);

    #ifdef DEBUG_LOG_ENABLE
        // std::cout << "Size path : " << std::endl;
    #endif

    }
}

void
VisualizeCloudPoint::show()
{
    

    // generate_cloud_widgets();

    while(!this->myWindow.wasStopped())
    {
        this->myWindow.spinOnce(1, true);
    }
}

#endif