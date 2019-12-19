#ifndef COURSETRACKER_HPP
#define COURSETRACKER_HPP

#include "opencv2/core.hpp"
#include "DataReader.hpp"
#include <string>

using namespace cv;

class CourseTracker {
private:
public:
    std::vector<std::vector<Matx22f>> key_points;

    CourseTracker(){}
    virtual ~CourseTracker(){}

    virtual void track_course(const size_t count_images, DataReader reader) = 0;
};

#endif
