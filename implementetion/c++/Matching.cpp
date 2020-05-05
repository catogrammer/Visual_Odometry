#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/ImageReader.hpp"
#include "PointTracker/CalibReader.hpp"
#include "PointTracker/VisualizeCloudPoint.hpp"

#define LENGHT_IMAGE_NAME 10

/** @function main */
int main( int argc, char** argv )
{
	StereoCourseTracker tracker;

	// std::string path_name = "/home/akuma/Visual_Odometry/implementetion/input_data/2011_09_26_drive_0048_sync/";
	std::string path_name = "/home/akuma/Downloads/Kitti_datasets/2011_09_26_drive_0018_sync/";
	ImageReader reader(path_name);

	CalibReader calib_data;
	calib_data.load_calib_data(path_name + "calib_cam_to_cam.txt");

	auto t1 = std::chrono::high_resolution_clock::now();

	tracker.track_course(3, reader, calib_data);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	std::cout << "Work time: " << duration * 1e-6 << " sec" << std::endl;

	cv::Mat K_m = cv::Mat(calib_data.calib_cam_data[0].K_xx(), true).reshape(1,3);
	std::pair<cv::Mat,cv::Mat> K_m_pair = std::make_pair(
		cv::Mat(calib_data.calib_cam_data[0].K_xx(), true).reshape(1,3),
		cv::Mat(calib_data.calib_cam_data[1].K_xx(), true).reshape(1,3));

	VisualizeCloudPoint viz_module(tracker.tracked_points, tracker.navigation_data, K_m_pair);
	viz_module.show();
	return 0;
}
