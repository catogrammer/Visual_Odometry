#include "opencv2/core.hpp"
#include "PointTracker/Transformation.hpp"

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

cv::Point3f
get_pose(std::vector<cv::Mat> triang_p, cv::Mat &t_prev)
{
	Transformation transf_m;
	cv::Mat R, t;

	cv::Mat Rt_mat = transf_m.get_Rt(triang_p);
	cv::Mat pose = t_prev;
	
	if (!Rt_mat.empty())
	{
		R = cv::Mat(Rt_mat, cv::Rect(0,0,3,3));
		t = cv::Mat(Rt_mat, cv::Rect(3,0,1,3)).t();
	}

    std::cout << "R:\n" << R << std::endl;
    std::cout << "t:\n" << t << std::endl;

	if (!R.empty())
	{
		pose = (R*(t_prev - t).t());
		t_prev = t;
	}

	return cv::Point3f(pose);
}


int main(int argc, char const *argv[])
{
    float arr[4][3] = {{1., 4., 5.},{2., 3., 1.},{4., 1., 3.},{0., 3., 3.}};
    float arr2[4][3] = {{1,1,1},{1,1,1},{1,1,1},{1,1,1}};
    cv::Mat t_prev = cv::Mat(cv::Point3f(0,0,0)).t();
    cv::Mat test_m = cv::Mat(4, 3, CV_32F, &arr);
    test_m.convertTo(test_m, CV_64F);
    cv::Mat t = cv::Mat(4, 3, CV_32F, &arr2);
    t.convertTo(t, CV_64F);
    t_prev.convertTo(t_prev, CV_64F);
    cv::Mat ones = cv::Mat::ones(4, 1, CV_64F);

    std::cout << "beg p :" << std::endl <<
    test_m << std::endl << "t : \n" << t_prev.type() << std::endl;
    
    cv::Mat test_m2 = (test_m + t);

    std::vector<cv::Mat> test_p = {test_m.t(), test_m2.t()};

    std::cout << "beg p :" << std::endl << test_m2 << std::endl;
    std::cout << "size :" << test_m.size()  << "=?" << test_m2.size() <<  std::endl;

    std::cout << "TEST" << std::endl;
    std::cout << "Pose : \n" << get_pose(test_p, t_prev) << std::endl;

    return 0;
}
