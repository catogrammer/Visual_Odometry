#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/ImageReader.hpp"
#include "PointTracker/SimpleCalibReader.hpp"
#include "PointTracker/VisualizeCloudPoint.hpp"
#include "PointTracker/TruePathReader.hpp"

// #define COUNT_IMAGES 10

/** @function main */
int main( int argc, char** argv )
{
	cv::String keys =
        "{@image |<none>           | input image sequence path}"         // input image is the first argument (positional)
        "{@count_of_imgs   |10 | count of fetures}"         // optional, default value ""
        "{count_of_features   |1500 | count of fetures}"      
        "{ratio_thresh   |0.7     |  ratio test value}"       // optional, default value ""
        "{help   |      | show help message}";      // optional, show help optional

    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }
    cv::String input_image_path = parser.get<cv::String>(0); // read @image (mandatory, error if not present)
    int count_of_imgs = parser.get<int>(1); // read @face (use default value if not in cmd)
    int count_of_fetures = parser.get<int>("count_of_features"); // read @face (use default value if not in cmd)
    float ratio_thresh = parser.get<float>("ratio_thresh");

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }


    std::cout << "input_image_path : " << input_image_path << std::endl
              << "count_of_imgs : " << count_of_imgs << std::endl
              << "count_of_fetures : " << count_of_fetures << std::endl
              << "ratio_thresh : " << ratio_thresh << std::endl;

	StereoCourseTracker tracker;

	// std::string path_name = "/home/akuma/Downloads/Kitti_datasets/2011_09_26_drive_0018_sync/";
	// std::string path_name = "/home/akuma/Downloads/Kitti_datasets/2011_09_26_drive_0048_sync/";
	// std::string path_name = "/home/akuma/Downloads/Kitti_datasets/2011_09_26_drive_0117_sync/";
	std::string path_name = input_image_path;
	ImageReader reader(path_name);

	SimpleCalibReader calib_data;
	calib_data.load_calib_data(path_name + "calib.txt");

	auto t1 = std::chrono::high_resolution_clock::now();

	tracker.track_course(count_of_imgs, count_of_fetures, reader, calib_data);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	std::cout << "Work is done! " << std::endl
			  << "Work time: " << duration * 1e-6 << " sec" << std::endl;

	TruePathReader truth_p(path_name + "01.txt");
	if (!truth_p.read_data(count_of_imgs))
		std::cout << "True path wasn't load!" << std::endl;

	tracker.test(truth_p);

	cv::Mat K_m_0 = cv::Mat(calib_data.calib_cam_data[0].P_xx(), true).reshape(1,3);
	cv::Mat K_m_1 = cv::Mat(calib_data.calib_cam_data[1].P_xx(), true).reshape(1,3);
	K_m_0 = cv::Mat(K_m_0, cv::Rect(0,0,3,3));
	K_m_1 = cv::Mat(K_m_1, cv::Rect(0,0,3,3));
	std::cout << "Intric mat cam 0: \n" << K_m_0 << "\n Intric mat cam 1: \n" << K_m_1 << std::endl;
	std::pair<cv::Mat,cv::Mat> K_m_pair = std::make_pair(K_m_0, K_m_1);

	VisualizeCloudPoint viz_module(tracker.tracked_points);
	std::cout << "WTFF!?" << std::endl;	
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

	cv::Point3f all_path_truth(0,0,0);
	cv::Point3f all_path_find(0,0,0);
	cv::Point3f err(0,0,0);
	for (size_t i = 0; i < tracker.navigation_data.size(); i++)
	{
		all_path_truth += truth_p.poses[i];
		all_path_find += tracker.navigation_data[i];
	}
	err = all_path_truth - all_path_find;
	err = cv::Point3f(err.x/all_path_truth.x, err.y/all_path_truth.y, err.z/all_path_truth.z);
	std::cout << "True path : " << all_path_truth << std::endl
			  << "Find path : " << all_path_find << std::endl
			  << "Error : " << err << std::endl;  
	return 0;
}
