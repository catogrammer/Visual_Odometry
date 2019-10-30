#ifndef STEREOPOINTTRACKER_HPP
#define STEREOPOINTTRACKER_HPP

#include "PointTracker.hpp"

#define RATIO_THRESH 0.61f

// template <class Detector, class Descriptor, class Matcher>
class StereoPointTracker : public PointTracker {
private:
public:
	std::vector<KeyPoint>				kps_l, kps_r;
	std::vector<std::vector<DMatch>>	knn_matches;
	std::vector<DMatch> 				good_matches;

	Mat descriptors_l, descriptors_r;
	Mat image_l, image_r;

	StereoPointTracker(Mat image_l, Mat image_r, std::vector<KeyPoint> kps_l, std::vector<KeyPoint> kps_r): PointTracker(), image_l(image_l), image_r(image_r), kps_l(kps_l), kps_r(kps_r){}

	void detect_features();
	void match_features();
	void get_good_matches();
	std::vector<Matx22f> get_good_coordinate();
	// void get_points_position();
};

// template <class Detector>
void
StereoPointTracker::detect_features(){
	Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(FastFeatureDetector::TYPE_9_16);

	detector->detect(image_l, kps_l);
	detector->detect(image_r, kps_r);
}

// template <class Descriptor, Matcher>
void
StereoPointTracker::match_features(){
	Ptr<DAISY> computer = DAISY::create(DAISY::NRM_NONE);
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);

	computer->compute(image_l, kps_l, descriptors_l);
	computer->compute(image_r, kps_r, descriptors_r);
	matcher->knnMatch(descriptors_l, descriptors_r, knn_matches, 2);
}

void
StereoPointTracker::get_good_matches(){
	// Filter matches using the Lowe's ratio test
	const float ratio_thresh = 0.79f;
	for (size_t i = 0; i < knn_matches.size(); i++){
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance){
			good_matches.push_back(knn_matches[i][0]);
		}
	}
}

// template <typename Point> Point
// get_point_position(Point point_0_a, Point point_1_a,
//            Point point_0_b, Point point_1_b)
// {
//     Point across_point;
//     Point p_a = point_1_a - point_0_a;
//     Point p_b = point_1_b - point_0_b;
//     double t = (point_0_b[0] - point_0_a[0])/(p_a[0] - p_b[0]);
//
//     across_point << p_a*t + point_0_a;
//     return across_point;
// }

std::vector<Matx22f>
StereoPointTracker::get_good_coordinate(){
	std::vector<Matx22f> key_points;
	for (auto el : good_matches){
		Point2f l_point = kps_l[el.queryIdx].pt;
		Point2f r_point = kps_r[el.trainIdx].pt;
		Matx22f tmp(l_point.x, l_point.y,
					r_point.x, r_point.y);
		key_points.push_back(tmp);
	}
	return key_points;
}

// void
// PointTracker::get_matched_features()
// {
// 	// KeyPoints stroerd Point2f type
// 	for (auto el : good_matches)
// 	{
// 		std::cout << keypoints_1[el.queryIdx].pt << " "
// 				  << keypoints_2[el.trainIdx].pt << std::endl;
// 	}
// }

#endif
