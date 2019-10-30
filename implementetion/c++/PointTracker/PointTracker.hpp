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

// template <class Detector, class Descriptor, class Matcher>
class PointTracker {
public:
	PointTracker(){}
	virtual ~PointTracker(){}

	virtual void detect_features() = 0;
	virtual void match_features() = 0;
	// virtual void match_features(std::vector<KeyPoint> kp_l, std::vector<KeyPoint> kp_r) = 0;
	// virtual void get_points_position() = 0;
};

#endif
