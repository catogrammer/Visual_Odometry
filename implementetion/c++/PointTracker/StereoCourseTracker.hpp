#ifndef STEREOCOURSETRACKER_HPP
#define STEREOCOURSETRACKER_HPP

#include "CourseTracker.hpp"
#include "StereoPointTracker.hpp"

// using namespace cv;

class StereoCourseTracker : protected CourseTracker {
private:
public:
    StereoCourseTracker() : CourseTracker(){}
    ~StereoCourseTracker(){}

    void track_course(const size_t count_images, DataReader reader);
};

void
StereoCourseTracker::track_course(const size_t count_images, DataReader reader){
    std::vector<KeyPoint> left_keypoints, right_keypoints;
    for (size_t i = 0; i < count_images; i++) {
        Mat curr_img_left, curr_img_right;
        // std::vector<KeyPoint> left_keypoints, right_keypoints;

        reader.read_pair_image(curr_img_left, curr_img_right, i);
        StereoPointTracker tracker(curr_img_left, curr_img_right);
        tracker.detect_features();
        tracker.match_features();
        // tracker.get_
        // tracker.get_key_poits(left_keypoints, right_keypoints)
        // curr_point = get_points_position();

    	// tracker.match_features();
        // curr_point = get_points_position();
        // key_points.pushback(curr_point);

        // left_keypoints.clear();
        // right_keypoints.clear()
    }
}

#endif
