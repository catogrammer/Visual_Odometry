#ifndef COURSETRACKER_HPP
#define COURSETRACKER_HPP

#include <string>

#include "opencv2/core.hpp"
#include "ImageReader.hpp"
#include "SimpleCalibReader.hpp"

using namespace cv;

class CourseTracker {
private:
public:
    std::vector<std::vector<Matx22f>> key_points;

    CourseTracker(){}
    virtual ~CourseTracker(){}

    virtual void track_course(const size_t count_images,
                              const size_t cnt_features, ImageReader reader,
                              SimpleCalibReader calib_data) = 0;
};

#endif
