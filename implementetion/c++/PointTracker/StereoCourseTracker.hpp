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
    std::vector<KeyPoint> left_kps, right_kps;

    for (size_t i = 0; i < count_images; i++) {
        Mat curr_img_left, curr_img_right;
        std::vector<Matx22f> tmp;

        reader.read_pair_image(curr_img_left, curr_img_right, i);
        StereoPointTracker tracker(curr_img_left, curr_img_right,
        left_kps, right_kps);
        if (i == 0)
            tracker.detect_features();
        tracker.match_features();
        left_kps.clear();
        right_kps.clear();
        left_kps  = tracker.kps_l;
        right_kps = tracker.kps_r;
        tracker.get_good_matches();
        tmp = tracker.get_good_coordinate();
        std::cout << "size of good_matches = " << tmp.size() << std::endl;
        this->key_points.push_back(tmp);
    }
}

#endif
