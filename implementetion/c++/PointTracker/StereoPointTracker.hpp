#ifndef STEREOPOINTTRACKER_HPP
#define STEREOPOINTTRACKER_HPP

#include "PointTracker.hpp"

#define RATIO_THRESH 0.69f

// template <class Detector, class Descriptor, class Matcher>
class StereoPointTracker : public PointTracker {
private:
public:
	std::vector<KeyPoint>				kps_l, kps_r;
	std::vector<std::vector<DMatch>>	knn_matches;
	std::vector<DMatch> 				good_matches;

	Mat descriptors_l, descriptors_r;
	Mat image_l, image_r;

	StereoPointTracker(Mat image_l,
					   Mat image_r)
		: PointTracker(), image_l(image_l), image_r(image_r){}

	StereoPointTracker(Mat image_l,
					   Mat image_r,
					   std::vector<KeyPoint> kps_l,
					   std::vector<KeyPoint> kps_r) :
					   PointTracker(), image_l(image_l),
					   image_r(image_r),
					   kps_l(kps_l),
					   kps_r(kps_r) {}


	void detect_features();
	void match_features();
	void get_good_matches();
	std::vector<Matx22f> get_good_coordinate();
	std::vector<Point3f> get_result_point_positions(Point3f l_cam_p,
													Point3f r_cam_p);
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
	// std::cout << "size knn = "<< knn_matches.size() << std::endl;
}

void
StereoPointTracker::get_good_matches(){
	// Filter matches using the Lowe's ratio test
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < RATIO_THRESH * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
}

/**
 *
 * @param l_cam_p		Left cam position
 * @param r_cam_p		Right cam position
 * @param key_point		Vector of keypoints,
 						which stored matrix with
						coordinate features of left
						and right images
 */
std::vector<Point3f>
StereoPointTracker::get_result_point_positions(Point3f l_cam_p,
											   Point3f r_cam_p)
{
	std::vector<Matx22f> key_points = get_good_coordinate();
	std::vector<Point3f> result;

	for (auto el : key_points)
	{
		float t = 0;
		Point3f across_point;
		Point3f p_left  = Point3f(el(0,0), el(0,1), 1) - l_cam_p;
		Point3f p_right = Point3f(el(1,0), el(1,1), 1) - r_cam_p;

		t = (r_cam_p.x - l_cam_p.x)/(p_left.x - p_right.x);
		across_point = p_left*t + l_cam_p;
		result.push_back(across_point);
	}
	return result;
}

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
// 		std::cout << kps_l[el.queryIdx].pt << " "
// 				  << kps_r[el.trainIdx].pt << std::endl;
// 	}
// }

#endif
