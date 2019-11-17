#ifndef STEREOCOURSETRACKER_HPP
#define STEREOCOURSETRACKER_HPP

#include "CourseTracker.hpp"
#include "StereoPointTracker.hpp"

// using namespace cv;

class StereoCourseTracker : protected CourseTracker {
private:

public:
    std::vector<Point2f> key_points;

    StereoCourseTracker() : CourseTracker(){}
    ~StereoCourseTracker(){}

    void track_course(const size_t count_images, DataReader reader);
};

void
StereoCourseTracker::track_course(const size_t count_images, DataReader reader)
{
    std::vector<KeyPoint> curr_left_kps, curr_right_kps;

    for (size_t i = 0; i < count_images; i++) {
        Mat curr_img_left, curr_img_right;

        reader.read_pair_image(curr_img_left, curr_img_right, i);
        StereoPointTracker tracker(curr_img_left, curr_img_right,
                                   curr_left_kps, curr_right_kps);

        if (i == 0)
            tracker.detect_features();

        tracker.match_features();
        tracker.get_good_matches();

        // std::vector<Matx22f> tmp_kps = tracker.get_good_coordinate();

        // key_points.push_back(tracker.get_result_point_positions());
        Point3f l_cam_p(-0.27, 0, 1);
        Point3f r_cam_p(0.27, 0, 1);
        std::vector<Point3f> tmp = tracker.get_result_point_positions(l_cam_p, r_cam_p);
        for (auto el : tmp)
        {
            std::cout << '[' << el.x << ',' << el.y << ' ' << el.z << ']' << std::endl;
        }
	    std::cout << "size features = " << tmp.size() << std::endl;
        //save detected features
        curr_left_kps.clear();
        curr_right_kps.clear();
        curr_left_kps  = tracker.kps_l;
        curr_right_kps = tracker.kps_r;
        //save DMatch vector

    }
}

#endif
