#ifndef POINTTRACKER_HPP
#define POINTTRACKER_HPP

#include <stdio.h>
#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

/**
 * must store all detected point on all image and
 */

// template <class Detector, class Descriptor, class Matcher>
class MonoPointTracker {
public:
	std::vector<KeyPoint>				keypoints_1, keypoints_2;
	std::vector< std::vector<DMatch> >	knn_matches;
	std::vector<DMatch> 				good_matches;

	Mat descriptors_1, descriptors_2;
	Mat image_1, image_2;


	MonoPointTracker(Mat img_1, Mat img_2): image_1(img_1), image_2(img_2){}

	void detect_features();
	void match_features();
	// void get_matched_features();
	void get_points_position();
};

// template <class Detector>
void
MonoPointTracker::detect_features()
{
	Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(FastFeatureDetector::TYPE_9_16);

	detector->detect(image_1, keypoints_1);
	detector->detect(image_2, keypoints_2);
}

// template <class Descriptor, Matcher>
void
MonoPointTracker::match_features()
{
	Ptr<DAISY> computer = DAISY::create(DAISY::NRM_NONE);
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);

	computer->compute(image_1, keypoints_1, descriptors_1);
	computer->compute(image_2, keypoints_2, descriptors_2);

	matcher->knnMatch(descriptors_1, descriptors_2, knn_matches, 2);

	// Filter matches using the Lowe's ratio test
	const float ratio_thresh = 0.79f;

	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
}

// void
// MonoPointTracker::get_matched_features()
// {
// 	// KeyPoints stroerd Point2f type
// 	for (auto el : good_matches)
// 	{
// 		std::cout << keypoints_1[el.queryIdx].pt << " "
// 				  << keypoints_2[el.trainIdx].pt << std::endl;
// 	}
// }

// 
// void
// get_points_position()
// {
//
// }

#endif
