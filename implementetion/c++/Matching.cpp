#include <stdio.h>
#include <iostream>
#include <string>

#include "PointTracker/StereoCourseTracker.hpp"
#include "PointTracker/DataReader.hpp"

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
