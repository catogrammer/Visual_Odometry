#include "opencv2/core.hpp"
#include <opencv2/sfm/triangulation.hpp>

#include <vector>
#include <iostream>

template<typename T>
void print_vec(std::vector<T> vec){
    for (auto &&i : vec)
    {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    cv::Mat clear_data;
    cv::Mat data_1 = (cv::Mat_<int>(3,2) << 1, 2, 3, 4, 5, 6);
    cv::Mat data_2 = (cv::Mat_<int>(3,2) << 2, 5, 6, 1, 9, 7);

    cv::absdiff(data_1, data_2, clear_data);

    cv::Scalar menan_;
    cv::Scalar stddev_;
    cv::meanStdDev(clear_data.col(0), menan_, stddev_);
    std::cout << "mean : " << menan_ << std::endl
              << "stddev : " << stddev_ << std::endl;
    cv::meanStdDev(clear_data.col(1), menan_, stddev_);
    std::cout << "mean : " << menan_ << std::endl
              << "stddev : " << stddev_ << std::endl;
    std::cout << clear_data << std::endl;


    // std::vector<cv::Mat> in_p;
    // cv::Mat out_p;

    // std::vector<cv::Point2d> tmp_1{cv::Point2d(100,200),
    //                                cv::Point2d(400,200),
    //                                cv::Point2d(600,200)};
    // std::vector<cv::Point2d> tmp_2{cv::Point2d(200,300),
    //                                cv::Point2d(500,300),
    //                                cv::Point2d(700,300)};
    
    // cv::Mat points1Mat = cv::Mat(2, tmp_1.size(), CV_64F, tmp_1.data());
    // cv::Mat points2Mat = cv::Mat(2, tmp_2.size(), CV_64F, tmp_2.data());
    
    // /*
    // P_rect_00: 7.215377e+02 0.000000e+00 6.095593e+02 0.000000e+00
    //            0.000000e+00 7.215377e+02 1.728540e+02 0.000000e+00
    //            0.000000e+00 0.000000e+00 1.000000e+00 0.000000e+00
    // */
    // // cv::Mat p_mat = (cv::Mat_<double>(3,4) << 400, 0,    610,  0,
    // //                                           0,   400,  170, 0,
    // //                                           0,   0,    1,    0);
    // std::vector<float> tmp{721.538, 0, 609.559, 0, 0, 721.538, 172.854, 0, 0, 0, 1, 0};
    // cv::Mat p_mat = cv::Mat(tmp, true);
    // p_mat = p_mat.reshape(1,3);

    // std::cout << p_mat << std::endl;
    // std::cout << p_mat.size << std::endl;

    // std::vector<cv::Mat> proj_mtrx{p_mat, p_mat};
    
    // in_p.push_back(points1Mat);
    // in_p.push_back(points2Mat);
    // std::cout << points1Mat << std::endl;
    // std::cout << points2Mat << std::endl;

    // cv::sfm::triangulatePoints(in_p, proj_mtrx, out_p);

    // std::cout << out_p << std::endl;
    // // std::cout << out_p.size << std::endl;

    return 0;
}
