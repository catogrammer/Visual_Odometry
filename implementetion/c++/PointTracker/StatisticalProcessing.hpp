#ifndef STATISTICALPROCESSING_HPP
#define STATISTICALPROCESSING_HPP

#include <vector>
#include <iostream>
#include "opencv2/core.hpp"

class StatisticalProcessing
{
private:
    cv::Mat data;
public:
    cv::Scalar mean;
    cv::Scalar stddev;
    
    StatisticalProcessing(cv::Mat d): data(d) {};
    ~StatisticalProcessing() {};
    
    void prepare_data();
    cv::Scalar prepare_data(cv::Mat data);
};

void
StatisticalProcessing::prepare_data(){

    for (size_t i = 0; i < data.cols; i++){
        cv::Scalar mean_;
        cv::Scalar stddev_;
        cv::meanStdDev(data.col(i), mean_, stddev_);
        this->mean[i] = mean_[0];
        this->stddev[i] = stddev_[0];
    }

#ifdef DEBUG_LOG_ENABLE	
    std::cout << "mean : " << this->mean << std::endl
              << "stddev : " << this->stddev << std::endl;
#endif

}

cv::Scalar
StatisticalProcessing::prepare_data(cv::Mat data_){
    cv::Scalar mean_l;
    cv::Scalar stddev_l;

    for (size_t i = 0; i < data_.cols; i++){
        cv::Scalar mean_;
        cv::Scalar stddev_;
        cv::meanStdDev(data_.col(i), mean_, stddev_);
        mean_l[i] = mean_[0];
        stddev_l[i] = stddev_[0];
    }

    return mean_l;
}

#endif