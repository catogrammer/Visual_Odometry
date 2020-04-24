#include <opencv2/viz/vizcore.hpp>
#include <iostream>



int main()
{
    // Create a window
    cv::viz::Viz3d myWindow("Viz Demo");

	// Add coordinate axes
    myWindow.showWidget("Coordinate", cv::viz::WCoordinateSystem());

	// Add cloud widget
	// float arr[] = {.1, .3, .1, .5, .3, .8, .6, .2, .9, };
	size_t n = 9;
	float *arr = new float[n];
	for (int i=0; i<n; i++) {
		arr[i] = (i+1.0)/10;
	}

	cv::Mat cloudMat = cv::Mat(1, 3, CV_32FC3, arr);
	std::cout << cloudMat << std::endl;
	cv::viz::WCloud cloud_widget(cloudMat, cv::viz::Color::white());
	cloud_widget.setRenderingProperty( cv::viz::POINT_SIZE, 4 );
	myWindow.showWidget("Cloud", cloud_widget);

	// /// Construct a cube widget
    // cv::viz::WCube cube_widget(cv::Point3f(0.5,0.5,0.0),
	// 						   cv::Point3f(0.0,0.0,-0.5),
	// 						   true, cv::viz::Color::blue());
    // cube_widget.setRenderingProperty(cv::viz::LINE_WIDTH, 4.0);
	// myWindow.showWidget("Cube", cube_widget);

    while(!myWindow.wasStopped())
    {
		// cv::viz::WCloud cloud_widget(cloudMat, cv::viz::Color::white());
		// cloud_widget.setRenderingProperty( cv::viz::POINT_SIZE, 4 );
		// myWindow.showWidget("Cloud", cloud_widget);
        // Event loop for 1 millisecond
        myWindow.spinOnce(33, true);
    }
    return 0;
}