#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

void readme();

/** @function main */
int main( int argc, char** argv )
{
    if( argc != 4 )
    { readme(); return -1; }

    Mat img_1 = imread( argv[1], IMREAD_GRAYSCALE );
    Mat img_2 = imread( argv[2], IMREAD_GRAYSCALE );
    Mat img_3 = imread( argv[3], IMREAD_GRAYSCALE );

    if( !img_1.data || !img_2.data ||  !img_3.data)
    { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

    //-- Step 1: Detect the keypoints

    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(FastFeatureDetector::TYPE_9_16);

    std::vector<KeyPoint> keypoints_1, keypoints_2;

    detector->detect( img_1, keypoints_1 );
    detector->detect( img_2, keypoints_2 );

    // std::cout << "size of vector_1 = " << keypoints_1.size() << std::endl;
    // std::cout << "size of vector_2 = " << keypoints_2.size() << std::endl;

    //-- Step 2: Match the keypoints
    Mat descriptors_1, descriptors_2, descriptors_3;
    Ptr<DAISY> computer = DAISY::create(DAISY::NRM_NONE);
    computer->compute(img_1, keypoints_1, descriptors_1);
    computer->compute(img_2, keypoints_2, descriptors_2);

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches, knn_matches_2;
    matcher->knnMatch(descriptors_1, descriptors_2, knn_matches, 2);
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.59f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    std::vector<cv::KeyPoint> keypoints_3;
    for (auto el : good_matches)
    {
        std::cout << keypoints_1[el.queryIdx].pt << " "
                  << keypoints_2[el.trainIdx].pt << std::endl;

        keypoints_3.push_back(keypoints_2[el.trainIdx]);
    }
    std::cout << "*********************" << std::endl;
    computer->compute(img_3, keypoints_3, descriptors_3);
    matcher->knnMatch(descriptors_2, descriptors_3, knn_matches_2, 2);

    std::vector<DMatch> good_matches_2;
    for (size_t i = 0; i < knn_matches_2.size(); i++)
    {
        if (knn_matches_2[i][0].distance < ratio_thresh * knn_matches_2[i][1].distance)
        {
            good_matches_2.push_back(knn_matches_2[i][0]);
        }
    }

    for (auto el : good_matches_2)
    {
        std::cout << keypoints_2[el.queryIdx].pt << " "
                  << keypoints_3[el.trainIdx].pt << std::endl;
        // continue;
    }

    std::cout << "size of good_matches = " << good_matches.size() << std::endl;
    std::cout << "size of good_matches_2 = " << good_matches_2.size() << std::endl;

    //-- Draw matches
    Mat img_matches;
    // drawMatches( img_1, keypoints_1, img_2, keypoints_2, good_matches, img_matches, Scalar::all(-1),
    //              Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    // //-- Show detected matches
    // imshow("Good Matches", img_matches );
    waitKey();
    return 0;
}

/** @function readme */
void readme()
{ std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }
