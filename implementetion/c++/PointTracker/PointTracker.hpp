#ifndef POINTTRACKER_HPP
#define POINTTRACKER_HPP

#include <stdio.h>
#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

// template <class Detector, class Descriptor, class Matcher>
class PointTracker {
public:
	PointTracker(){}
	virtual ~PointTracker(){}

	virtual void detect_features() = 0;
	virtual void match_features() = 0;
};

#endif
