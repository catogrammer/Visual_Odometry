#ifndef STATISTICALPROCESSING_HPP
#define STATISTICALPROCESSING_HPP

#include <vector>
#include <iostream>
#include "opencv2/core.hpp"

class StatisticalProcessing
{
private:
    std::vector<cv::Mat> data;
    cv::Mat clear_data;
    cv::Scalar mean;
    cv::Scalar stddev;
public:
    StatisticalProcessing(std::vector<cv::Mat> d): data(d) {};
    ~StatisticalProcessing() {};

    cv::Mat prepare_data();
};

cv::Mat
StatisticalProcessing::prepare_data(){
    cv::absdiff(data[0], data[1], clear_data);
    
    for (size_t i = 0; i < clear_data.cols; i++){
        cv::Scalar mean_;
        cv::Scalar stddev_;
        cv::meanStdDev(clear_data.col(i), mean_, stddev_);
        this->mean[i] = mean_[0];
        this->stddev[i] = stddev_[0];
    }

    std::cout << "mean : " << this->mean << std::endl
              << "stddev : " << this->stddev << std::endl;

    // applay 3 sigma rule
    const int n_sigma = 3;
    cv::Mat res_points;
    cv::subtract(this->mean, clear_data, clear_data);
    for (size_t i = 0; i < clear_data.rows; i++){
        if (clear_data.at<double>(i,0) < n_sigma * this->stddev[0] &&
            clear_data.at<double>(i,1) < n_sigma * this->stddev[1] &&
            clear_data.at<double>(i,2) < n_sigma * this->stddev[2] 
            ){
                res_points.push_back(clear_data.row(i));
            }
    }
    return res_points;
}

#endif