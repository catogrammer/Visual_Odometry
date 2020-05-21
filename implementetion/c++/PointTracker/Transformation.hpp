#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include "opencv2/core.hpp"
#include <iostream>

class Transformation
{
private:

public:
    Transformation(){};
    ~Transformation(){};

    cv::Mat get_Rt(std::vector<cv::Mat> curr_next_p);
};

cv::Mat
Transformation::get_Rt(std::vector<cv::Mat> curr_next_p)
{
    cv::Mat transform;
    cv::Mat X1;
    cv::Mat X2;
   
    size_t cnt_curr_p = curr_next_p[0].cols;
    size_t cnt_next_p = curr_next_p[1].cols;
    cv::Mat ones = cv::Mat::ones(1, cnt_curr_p, CV_64F);

    if (cnt_curr_p > 0 && cnt_next_p > 0)
    {
        cv::vconcat(curr_next_p[0], ones, X1);
        cv::vconcat(curr_next_p[1], ones, X2);
    }

    if (cnt_curr_p >= 4 and cnt_next_p >=4)
        transform = X2 * X1.t() * (X1*X1.t()).inv();

    return transform;
}


#endif