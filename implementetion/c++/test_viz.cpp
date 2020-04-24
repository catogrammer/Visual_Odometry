#include <opencv2/viz/vizcore.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/**
 * @function main
 */
int main()
{
    /// Create a window
    viz::Viz3d myWindow("Coordinate Frame");

    // Add coordinate axes
    myWindow.showWidget("Coordinate Widget", viz::WCoordinateSystem());

    float arr[] = {.1, .3, .1, .5, .3, .8, .6, .2, .9, .1};
	Mat cloudMat(3, 2, CV_64FC4, arr);
    // cloudMat << .1, .3, .1, .5, .3, .8, .6, .2, .9;
	

    /// Construct a cube widget
    // viz::WCube cube_widget(Point3f(0.5,0.5,0.0), Point3f(0.0,0.0,-0.5), true, viz::Color::blue());
    // cube_widget.setRenderingProperty(viz::LINE_WIDTH, 2.0);

    // /// Display widget (update if already displayed)
    // myWindow.showWidget("Cube Widget", cube_widget);

    /// Rodrigues vector
    Mat rot_vec = Mat::zeros(1,3,CV_32F);
    float translation_phase = 0.0, translation = 0.0;
    while(!myWindow.wasStopped())
    {
        /* Rotation using rodrigues */
        /// Rotate around (1,1,1)
        rot_vec.at<float>(0,0) += CV_PI * 0.01f;
        rot_vec.at<float>(0,1) += CV_PI * 0.01f;
        rot_vec.at<float>(0,2) += CV_PI * 0.01f;

        /// Shift on (1,1,1)
        translation_phase += CV_PI * 0.01f;
        translation = sin(translation_phase);

        Mat rot_mat;
        Rodrigues(rot_vec, rot_mat);

        /// Construct pose
        Affine3f pose(rot_mat, Vec3f(translation, translation, translation));


        viz::WCloud cloud_widget(cloudMat, viz::Color::white());
        cloud_widget.setRenderingProperty( viz::POINT_SIZE, 4 );
        myWindow.showWidget("Cloud Widget", cloud_widget);
        
        // myWindow.setWidgetPose("Cube Widget", pose);

        myWindow.spinOnce(1, true);
    }

    return 0;
}