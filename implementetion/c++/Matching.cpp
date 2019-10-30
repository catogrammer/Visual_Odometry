#include <stdio.h>
#include <iostream>
#include <string>

#include "opencv2/core.hpp"
// #include "opencv2/features2d.hpp"
// #include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

#include "PointTracker/PointTracker.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

#define LENGHT_IMAGE_NAME 10

std::string
get_image_name(const size_t& index)
{
	std::string image_name;
	std::string str_index = std::to_string(index);
	for (size_t i = 0; i < LENGHT_IMAGE_NAME - str_index.length(); i++)
		image_name += '0';

	return image_name + str_index + ".png";
}

void
read_pair_image(Mat& img_1, Mat& img_2, const std::string& path,
				const std::string& image_name)
{
	std::string image_1_path = path + "image_02/" + "data/" + image_name;
	std::string image_2_path = path + "image_03/" + "data/" + image_name;

	std::cout << "path = " << image_1_path << std::endl;
	std::cout << "path = " << image_2_path << std::endl;

	img_1 = imread(image_1_path, IMREAD_COLOR);
	img_2 = imread(image_2_path, IMREAD_COLOR);

	if( !img_1.data || !img_2.data )
		std::cout << " --(!) Error reading images " << std::endl;
}

template <typename Point>
Point get_point_position(Point point_0_a, Point point_1_a,
           Point point_0_b, Point point_1_b)
{
    Point across_point;
    Point p_a = point_1_a - point_0_a;
    Point p_b = point_1_b - point_0_b;
    double t = (point_0_b[0] - point_0_a[0])/(p_a[0] - p_b[0]);

    across_point << p_a*t + point_0_a;
    return across_point;
}



/** @function main */
int main( int argc, char** argv )
{
	Mat img_1, img_2;
	const std::string image_name = get_image_name(0);
	const std::string path = "../../input_data/2011_09_26_drive_0013_sync/";
	read_pair_image(img_1, img_2, path, image_name);

	PointTracker tracker(img_1, img_2);
	tracker.detect_features();
	tracker.match_features();
	// tracker.visualise();
	
	//-- Draw matches
	Mat img_matches;
	drawMatches( img_1, tracker.keypoints_1, img_2, tracker.keypoints_2, tracker.good_matches, img_matches, Scalar::all(-1),
				 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
	//-- Show detected matches
	imshow("Good Matches", img_matches);
	waitKey();

	return 0;
}
