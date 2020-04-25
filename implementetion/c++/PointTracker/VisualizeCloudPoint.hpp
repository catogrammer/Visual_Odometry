#ifndef VISUALIZECLOUDPOINT_HPP
#define VISUALIZECLOUDPOINT_HPP

// #include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/viz/vizcore.hpp"

class VisualizeCloudPoint
{
private:
    std::pair<cv::Mat, cv::Mat> cloud_poits;
    cv::viz::Viz3d myWindow;
public:
    VisualizeCloudPoint(std::pair<cv::Mat, cv::Mat> cp): cloud_poits(cp){};
    ~VisualizeCloudPoint(){};

    void show_cp();
};

void
VisualizeCloudPoint::show_cp()
{
    this->myWindow = cv::viz::Viz3d("Viz Demo");
    cv::Mat cp_1 = this->cloud_poits.first.reshape(3, 1);
    cv::Mat cp_2 = this->cloud_poits.second.reshape(3, 1);

    cp_1.convertTo(cp_1, CV_32FC3);
    cp_2.convertTo(cp_2, CV_32FC3);
    
    cv::viz::WCloud cloud_1_widget(cp_1, cv::viz::Color::white());
    cv::viz::WCloud cloud_2_widget(cp_2, cv::viz::Color::purple());

	cloud_1_widget.setRenderingProperty(cv::viz::POINT_SIZE, 4);
	cloud_2_widget.setRenderingProperty(cv::viz::POINT_SIZE, 4);


    this->myWindow.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());
	this->myWindow.showWidget("Cloud 1 Widget", cloud_1_widget);
	this->myWindow.showWidget("Cloud 2 Widget", cloud_2_widget);

    while(!this->myWindow.wasStopped())
    {
        this->myWindow.spinOnce(1, true);
    }
}

#endif