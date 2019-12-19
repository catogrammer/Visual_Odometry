#include <stdio.h>
#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/calib3d.hpp"

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/DataReader.hpp"



using namespace cv;
using namespace cv::xfeatures2d;

#define LENGHT_IMAGE_NAME 10


/** @function main */
int main( int argc, char** argv )
{
	StereoCourseTracker tracker;
	DataReader reader("../../input_data/2011_09_26_drive_0013_sync/");
	// DataReader reader("../../input_data/");
	tracker.track_course(5, reader);
	
	return 0;
}
