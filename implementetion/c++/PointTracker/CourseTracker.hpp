#ifndef COURSETRACKER_HPP
#define COURSETRACKER_HPP

#include <string>

#include "opencv2/core.hpp"
#include "ImageReader.hpp"
#include "CalibReader.hpp"

using namespace cv;

class CourseTracker {
private:
public:
    std::vector<std::vector<Matx22f>> key_points;

    CourseTracker(){}
    virtual ~CourseTracker(){}

    virtual void track_course(const size_t count_images, ImageReader reader,
                              CalibReader calib_data) = 0;
};

#endif
