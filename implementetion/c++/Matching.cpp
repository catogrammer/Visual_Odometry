#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/ImageReader.hpp"
#include "PointTracker/CalibReader.hpp"
#include "PointTracker/VisualizeCloudPoint.hpp"
#include "PointTracker/TruePathReader.hpp"

#define COUNT_IMAGES 7

/** @function main */
int main( int argc, char** argv )
{
	StereoCourseTracker tracker;

	// std::string path_name = "/home/akuma/Visual_Odometry/implementetion/input_data/2011_09_26_drive_0048_sync/";
	std::string path_name = "/home/akuma/Downloads/Kitti_datasets/2011_09_26_drive_0048_sync/";
	ImageReader reader(path_name);

	CalibReader calib_data;
	calib_data.load_calib_data(path_name + "calib_cam_to_cam.txt");

	auto t1 = std::chrono::high_resolution_clock::now();

	tracker.track_course(COUNT_IMAGES, reader, calib_data);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	std::cout << "Work time: " << duration * 1e-6 << " sec" << std::endl;

	TruePathReader truth_p(path_name + "oxts/");
	if (!truth_p.read_data(COUNT_IMAGES))
		std::cout << "True path wasn't load!" << std::endl;
	truth_p.convertOxtsToPose();

	cv::Mat K_m = cv::Mat(calib_data.calib_cam_data[0].K_xx(), true).reshape(1,3);
	std::pair<cv::Mat,cv::Mat> K_m_pair = std::make_pair(
		cv::Mat(calib_data.calib_cam_data[0].K_xx(), true).reshape(1,3),
		cv::Mat(calib_data.calib_cam_data[1].K_xx(), true).reshape(1,3));

	VisualizeCloudPoint viz_module(tracker.tracked_points);
	viz_module.show_path(tracker.navigation_data, K_m_pair, cv::viz::Color::white(), true);
	viz_module.show_path(truth_p.poses, K_m_pair, cv::viz::Color::green(), true);
	viz_module.show();

	std::cout << "Truth path" << std::endl;
	for (auto &&i : truth_p.poses)
	{
		std::cout << i << std::endl;
	}

	std::cout << "My path" << std::endl;
	for (auto &&i : tracker.navigation_data)
	{
		std::cout << i << std::endl;
	}

	return 0;
}
