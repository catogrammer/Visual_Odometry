#ifndef VISUALIZECLOUDPOINT_HPP
#define VISUALIZECLOUDPOINT_HPP

#include "opencv2/core.hpp"
#include "opencv2/viz/viz3d.hpp"

class VisualizeCloudPoint
{
private:
    cv::Mat cloud_poits;
    cv::Point3DFrame point3DFrame;
public:
    VisualizeCloudPoint(cv::Mat cp): cloud_poits(cp){};
    ~VisualizeCloudPoint();
};

void
VisualizeCloudPoint::draw_c_p(){

}

#endif