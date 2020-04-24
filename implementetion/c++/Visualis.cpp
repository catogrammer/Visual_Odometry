#include <opencv2/viz/vizcore.hpp>
#include <iostream>



int main()
{
    // Create a window
    cv::viz::Viz3d myWindow("Viz Demo");

	// Add coordinate axes
    myWindow.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());

	cv::Mat3f cloudMat;
	cloudMat << 1.0f, 3.0f, 5.0f, 6.0f, 2.0f, 5.0f;

    while(!myWindow.wasStopped())
    {
		
		cv::viz::WCloud cloud(cloudMat, cv::viz::Color::blue());
		myWindow.showWidget("Cloud", cloud);
        /// Event loop for 30 millisecond
        myWindow.spinOnce(30, true);
    }
    return 0;
}