#ifndef COURSETRACKER_HPP
#define COURSETRACKER_HPP

#include "opencv2/core.hpp"
#include "string"

using namespace cv;

class CourseTracker {
private:
    /* data */

public:
    std::vector<Point3f>	key_points;

    CourseTracker ();

    void track_course();
};

void
CourseTracker::track_course(const size_t count_images)
{
    for (size_t i = 0; i < count_images - 1; i++) {
        Mat img_1, img_2;
        Point3f curr_point;

    	const std::string image_name = get_image_name(i);
    	const std::string path = "../../input_data/2011_09_26_drive_0013_sync/";

    	read_pair_image(img_1, img_2, path, image_name);
        PointTracker tracker(img_1, img_2);
    	tracker.detect_features();
    	tracker.match_features();
        curr_point = get_point_position();
        key_points.pushback(curr_point);
    }
}

#endif
