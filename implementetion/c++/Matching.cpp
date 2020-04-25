#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/ImageReader.hpp"
#include "PointTracker/CalibReader.hpp"

#define LENGHT_IMAGE_NAME 10

/** @function main */
int main( int argc, char** argv )
{
	StereoCourseTracker tracker;
	ImageReader reader("/home/akuma/Downloads/Kitti_datasets/2011_09_26/"
					   "2011_09_26_drive_0018_sync/");

	CalibReader calib_data;
	calib_data.load_calib_data("/home/akuma/Downloads/Kitti_datasets"
							   "/2011_09_26/calib_cam_to_cam.txt");

	auto t1 = std::chrono::high_resolution_clock::now();
	tracker.track_course(5, reader, calib_data);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	std::cout << "Work time: " << duration * 1e-6 << std::endl;

	// std::cout << calib_r.calib_cam_data[1].S_rect_xx() << std::endl;
	return 0;
}
