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

cv::Vec3d
CalculateMean(const cv::Mat_<cv::Vec3d> &points)
{
    cv::Mat_<cv::Vec3d> result;
    cv::reduce(points, result, 0, cv::REDUCE_AVG);
    return result(0, 0);
}

cv::Mat_<double>
FindRigidTransform(const cv::Mat_<cv::Vec3d> &points1, const cv::Mat_<cv::Vec3d> points2)
{
    /* Calculate centroids. */
    cv::Vec3d t1 = -CalculateMean(points1);
    cv::Vec3d t2 = -CalculateMean(points2);

    cv::Mat_<double> T1 = cv::Mat_<double>::eye(4, 4);
    T1(0, 3) = t1[0];
    T1(1, 3) = t1[1];
    T1(2, 3) = t1[2];

    cv::Mat_<double> T2 = cv::Mat_<double>::eye(4, 4);
    T2(0, 3) = -t2[0];
    T2(1, 3) = -t2[1];
    T2(2, 3) = -t2[2];

    /* Calculate covariance matrix for input points. Also calculate RMS deviation from centroid
     * which is used for scale calculation.
     */
    cv::Mat_<double> C(3, 3, 0.0);
    double p1Rms = 0, p2Rms = 0;
    for (int ptIdx = 0; ptIdx < points1.rows; ptIdx++) {
        cv::Vec3d p1 = points1(ptIdx, 0) + t1;
        cv::Vec3d p2 = points2(ptIdx, 0) + t2;
        p1Rms += p1.dot(p1);
        p2Rms += p2.dot(p2);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                C(i, j) += p2[i] * p1[j];
            }
        }
    }

    cv::Mat_<double> u, s, vh;
    cv::SVD::compute(C, s, u, vh);

    cv::Mat_<double> R = u * vh;

    if (cv::determinant(R) < 0) {
        R -= u.col(2) * (vh.row(2) * 2.0);
    }

    double scale = sqrt(p2Rms / p1Rms);
    R *= scale;

    cv::Mat_<double> M = cv::Mat_<double>::eye(4, 4);
    R.copyTo(M.colRange(0, 3).rowRange(0, 3));

    cv::Mat_<double> result = T2 * M * T1;
    result /= result(3, 3);

    return result.rowRange(0, 3);
}

cv::Mat
convet_mat_to_mat_vec(cv::Mat in)
{
    cv::Mat_<cv::Vec3d> out;
    for (size_t i = 0; i < in.cols; i++)
    {
        out.push_back(cv::Vec3d(in.col(i)));
    }

    return out;
}

cv::Point3f
get_pose_from_r(cv::Mat Rt_mat, cv::Mat &p_prev)
{
    cv::Mat pose = p_prev;
    p_prev.convertTo(p_prev, CV_64F);
	cv::Mat R, t;

	if (!Rt_mat.empty())
	{
		R = cv::Mat(Rt_mat, cv::Rect(0,0,3,3));
		t = cv::Mat(Rt_mat, cv::Rect(3,0,1,3)).t();
	}

    std::cout << "R: \n" << R << std::endl;
    std::cout << "t: \n" << t << std::endl;

	if (!R.empty())
	{
		pose = (R*(p_prev - t).t());
		p_prev = pose;
	}

	return cv::Point3f(pose);
}

int main(int argc, char const *argv[])
{

    cv::String keys =
        "{@image |<none>           | input image sequence path}"         // input image is the first argument (positional)
        "{@count_of_imgs   |10 | count of fetures}"         // optional, default value ""
        "{count_of_fetures   |1500 | count of fetures}"      
        "{ratio_thresh   |0.7     |  ratio test value}"       // optional, default value ""
        "{help   |      | show help message}";      // optional, show help optional


    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    cv::String input_image_path = parser.get<cv::String>(0); // read @image (mandatory, error if not present)
    int count_of_imgs = parser.get<int>(1); // read @face (use default value if not in cmd)
    int count_of_fetures = parser.get<int>("count_of_fetures"); // read @face (use default value if not in cmd)
    float ratio_thresh = parser.get<float>("ratio_thresh");



    // size_t cnt_of_fetures = std::stoi(count_of_fetures);


    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }


    std::cout << "input_image_path : " << input_image_path << std::endl
              << "count_of_imgs : " << count_of_imgs << std::endl
              << "count_of_fetures : " << count_of_fetures << std::endl
              << "ratio_thresh : " << ratio_thresh << std::endl;

    // float arr[4][3] = {{1., 4., 5.},{2., 3., 1.},{40., 1., 30.},{50., 3., 3.}};
    // float arr2[4][3] = {{2,2,2},{2,2,2},{2,2,2},{2,2,2}};
    // cv::softdouble a(CV_PI / 4);
    // float r[3][3] = {{cv::cos(a), -cv::sin(a), 0}, {cv::sin(a), cv::cos(a), 0}, {0,0,1}};
    // cv::Mat t_prev = cv::Mat(cv::Point3f(0,0,0)).t();

    // // cv::Mat test_m = cv::Mat(4, 3, CV_32F, &arr).t();
    // size_t cnt_p = 10;
    // cv::Mat test_m = cv::Mat(3, cnt_p, CV_32F);
    // cv::randu(test_m, cv::Scalar(-10), cv::Scalar(20));
    // cv::Mat rot_m = cv::Mat(3, 3, CV_32F, &r);
    // rot_m.convertTo(rot_m, CV_64F);

    // test_m.convertTo(test_m, CV_64F);
    // // cv::Mat t = cv::Mat(4, 3, CV_32F, &arr2).t();
    // cv::Mat t = cv::Mat(3, cnt_p, CV_32F, cv::Scalar(2));
    // t.convertTo(t, CV_64F);
    // t_prev.convertTo(t_prev, CV_64F);
    // cv::Mat ones = cv::Mat::ones(4, 1, CV_64F);
    
    // std::cout << "t : \n" << t << std::endl;
    // std::cout << "R :\n"  << rot_m << std::endl;
    // cv::Mat res_;
    // cv::Mat test_m2 = test_m + t;
    // for (size_t i = 0; i < test_m.cols; i++){
    //     res_.push_back((rot_m*test_m2.col(i)).t());
    // }

    // // std::vector<cv::Mat> test_p = {test_m, test_m2};
    // std::vector<cv::Mat> test_p = {test_m, res_.t()};
    // std::cout << "First points :" << std::endl << test_m << std::endl;
    // std::cout << "Second points :" << std::endl << res_.t() << std::endl;


    // // std::cout << "size :" << test_m.size()  << "=?" << test_m2.size() <<  std::endl;
    // // std::cout << "size :" << test_m.size()  << "=?" << test_m2.size() <<  std::endl;

    

 
    // std::cout << "TEST" << std::endl;
    // std::cout << "Pose 1 : \n" << get_pose(test_p, t_prev) << std::endl;
    // // std::cout << "----------MASQ ALGORITHM----------" << std::endl;
    // // t_prev = cv::Mat(cv::Point3f(0,0,0)).t();
    // // MSQ min_sq(test_p);
    // // std::cout << "Pose 2: \n" << min_sq.get_pose(t_prev) << std::endl;

    // std::cout << "TEST 2" << std::endl;
    // cv::Mat p1, p2;
    // t_prev = cv::Mat(cv::Point3f(0,0,0)).t();
    // p1 = convet_mat_to_mat_vec(test_m);
    // p2 = convet_mat_to_mat_vec(res_.t());
    // std::cout << "First points :" << std::endl << p1 << std::endl;
    // std::cout << "Second points :" << std::endl << p2 << std::endl;

    // cv::Mat Rt = FindRigidTransform(p1, p2);
    // std::cout << "solution : \n" <<  get_pose_from_r(Rt, t_prev) << std::endl;
    return 0;
}
