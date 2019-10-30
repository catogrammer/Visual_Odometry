#include <stdio.h>
#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/DataReader.hpp"



using namespace cv;
using namespace cv::xfeatures2d;

#define LENGHT_IMAGE_NAME 10


/** @function main */
int main( int argc, char** argv )
{
	// Mat img_1, img_2;
	// DataReader reader("../../input_data/");
	// reader.read_pair_image(img_1, img_2, 0);
	//
	// MonoPointTracker tracker(img_1, img_2);
	// tracker.detect_features();
	// tracker.match_features();
	// // tracker.visualise();
	//
	// //-- Draw matches
	// Mat img_matches;
	// drawMatches( img_1, tracker.keypoints_1, img_2, tracker.keypoints_2, tracker.good_matches, img_matches, Scalar::all(-1),
	// 			 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
	//-- Show detected matches
	// imshow("Good Matches", img_matches);
	// waitKey();

	StereoCourseTracker tracker;
	DataReader reader("../../input_data/");
	tracker.track_course(2, reader);

	return 0;
}
