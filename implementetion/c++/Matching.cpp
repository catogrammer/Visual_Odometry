#include <stdio.h>
#include <iostream>
#include <string>

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/ImageReader.hpp"
#include "PointTracker/CalibReader.hpp"

#define LENGHT_IMAGE_NAME 10

/** @function main */
int main( int argc, char** argv )
{
	StereoCourseTracker tracker;
	ImageReader reader("/home/akuma/Downloads/Kitti_datasets/2011_09_26/2011_09_26_drive_0018_sync/");

	CalibReader calib_r;
	calib_r.load_calib_data("/home/akuma/Downloads/Kitti_datasets/2011_09_26/calib_cam_to_cam.txt");
	tracker.track_course(10, reader);

	for (auto &&i : calib_r.calib_cam_data[2].S_xx())
	{
		std::cout << i << std::endl;
	}
	// std::cout << calib_r.calib_cam_data[1].S_rect_xx() << std::endl;
	return 0;
}
