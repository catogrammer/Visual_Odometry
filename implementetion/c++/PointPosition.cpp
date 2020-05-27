#include "opencv2/core.hpp"
#include <opencv2/core/softfloat.hpp>
#include "PointTracker/Transformation.hpp"
#include "PointTracker/MSQ.hpp"

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
        
        std::cout << "R:\n" << R << std::endl;
        std::cout << "t:\n" << t << std::endl;

	} else  std::cout << "Rt mat is empty" << std::endl;

   
	if (!R.empty())
	{
		pose = (R*(t_prev - t).t());
		t_prev = t;
	}

	return cv::Point3f(pose);
}


int main(int argc, char const *argv[])
{
    float arr[4][3] = {{1., 4., 5.},{2., 3., 1.},{40., 1., 30.},{50., 3., 3.}};
    float arr2[4][3] = {{2,2,2},{2,2,2},{2,2,2},{2,2,2}};
    cv::softdouble a(CV_PI / 4);
    float r[3][3] = {{cv::cos(a), -cv::sin(a), 0}, {cv::sin(a), cv::cos(a), 0}, {0,0,1}};
    cv::Mat t_prev = cv::Mat(cv::Point3f(0,0,0)).t();
    cv::Mat test_m = cv::Mat(4, 3, CV_32F, &arr).t();
    cv::Mat rot_m = cv::Mat(3, 3, CV_32F, &r);
    rot_m.convertTo(rot_m, CV_64F);

    test_m.convertTo(test_m, CV_64F);
    cv::Mat t = cv::Mat(4, 3, CV_32F, &arr2).t();
    t.convertTo(t, CV_64F);
    t_prev.convertTo(t_prev, CV_64F);
    cv::Mat ones = cv::Mat::ones(4, 1, CV_64F);
    
    std::cout << "t : \n" << t << std::endl;
    std::cout << "R :\n"  << rot_m << std::endl;
    cv::Mat res_;
    cv::Mat test_m2 = test_m + t;
    for (size_t i = 0; i < test_m.cols; i++){
        res_.push_back((rot_m*test_m2.col(i)).t());
    }

    std::cout << "beg p :" << std::endl <<
    test_m << std::endl;
    std::cout << "end p :" << std::endl <<
    res_.t() << std::endl;

    // std::vector<cv::Mat> test_p = {test_m, test_m2};
    std::vector<cv::Mat> test_p = {test_m, res_.t()};
    // std::cout << "end p :" << std::endl << test_m2.t() << std::endl;

    // std::cout << "size :" << test_m.size()  << "=?" << test_m2.size() <<  std::endl;
    // std::cout << "size :" << test_m.size()  << "=?" << test_m2.size() <<  std::endl;

    

 
    std::cout << "TEST" << std::endl;
    std::cout << "Pose 1 : \n" << get_pose(test_p, t_prev) << std::endl;
    std::cout << "----------MASQ ALGORITHM----------" << std::endl;
    t_prev = cv::Mat(cv::Point3f(0,0,0)).t();
    std::cout << "Prev pose " << t_prev << std::endl;
    MSQ min_sq(test_p, t_prev);
    std::cout << "Pose 2: \n" << min_sq.get_pose() << std::endl;

    return 0;
}
