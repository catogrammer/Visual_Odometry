#ifndef STEREOCOURSETRACKER_HPP
#define STEREOCOURSETRACKER_HPP

#include <utility>
#include <random>
#include <cmath>

#include "CourseTracker.hpp"
#include "StereoPointTracker.hpp"
#include "Transformation.hpp"
#include "TruePathReader.hpp"
#include "MSQ.hpp"

#include "opencv2/sfm/triangulation.hpp"
#include "opencv2/calib3d.hpp"
#include "StatisticalProcessing.hpp"

// #define DEBUG_LOG_ENABLE
// #define DEBUG_DRAW_ENABLE

class StereoCourseTracker : protected CourseTracker {
private:

public:
	std::vector<std::pair<std::vector<KeyPoint>, std::vector<KeyPoint>>> 
		key_points;
	std::vector<std::vector<DMatch>> good_matches;
	std::vector<std::pair<cv::Mat, cv::Mat>> tracked_points;
	std::vector<cv::Point3f> navigation_data = {cv::Point3f(0,0,0)};

	StereoCourseTracker() : CourseTracker(){}
	~StereoCourseTracker(){}

	void track_course(const size_t count_images, ImageReader reader, 
		CalibReader calib_data);
	std::vector<Mat> match_paired_points();
	std::vector<std::vector<cv::Point2f>>
		get_key_points_by_index(std::vector<cv::Mat> vec, size_t i);
	void print_paired_keypoints(std::vector<Mat> vec, size_t i);
	std::vector<cv::Mat> triangulate_matched_points(std::vector<Mat> indexes,
		size_t i, CalibReader calib_data);
	cv::Mat diff_between_points(std::vector<cv::Mat> c_and_p_points);
	cv::Point3f get_pose(std::vector<cv::Mat> triang_p, cv::Mat &t_prev);
	cv::Point3f min_sq(std::vector<cv::Mat> triang_p, cv::Mat &t_prev);

	void test(TruePathReader truth_p);
};

void
print_vector_dmatch(std::vector<std::vector<DMatch>> vec_dmatch)
{
	for(auto vec : vec_dmatch)
	{
		std::cout << "********" << std::endl;
		for (auto el : vec){
			std::cout << "[" << el.queryIdx << " "
					<< el.trainIdx << "]" << std::endl;
		}
	}
}

std::vector<Mat>
StereoCourseTracker::match_paired_points()
{
	std::vector<Mat> result;

	auto it_cletf_cright_beg = good_matches[0].begin(); //curr_left-curr_left
	auto it_cletf_cright_end = good_matches[0].end();
	auto it_cleft_nleft_beg  = good_matches[1].begin(); //curr_left-next_left
	auto it_cleft_nleft_end  = good_matches[1].end();
	auto it_nleft_nright_beg = good_matches[2].begin(); //curr_left-next_left
	auto it_nleft_nleft_end  = good_matches[2].end();

	for (; it_cletf_cright_beg != it_cletf_cright_end; )
	{
		Mat matched_indexes;
		
		if ((*it_cletf_cright_beg).queryIdx == (*it_cleft_nleft_beg).queryIdx )
		{
			matched_indexes = (-1)*Mat::ones(2,2, CV_32S);

			matched_indexes.at<int>(0,0) = (*it_cletf_cright_beg).queryIdx;
			matched_indexes.at<int>(0,1) = (*it_cletf_cright_beg).trainIdx;
			matched_indexes.at<int>(1,0) = (*it_cleft_nleft_beg).trainIdx;

			result.push_back(matched_indexes);

			it_cletf_cright_beg++;
			it_cleft_nleft_beg++;
		}
		else if ((*it_cletf_cright_beg).queryIdx < (*it_cleft_nleft_beg).queryIdx )
			it_cletf_cright_beg++;
		else
			it_cleft_nleft_beg++;
	}
	
	for (auto it_beg = result.begin(); it_beg != result.end(); )
	{
		if ((*it_beg).at<int>(1,0) == (*it_nleft_nright_beg).queryIdx )
		{
			(*it_beg).at<int>(1,1) = (*it_nleft_nright_beg).trainIdx;
			it_beg++;
		}
		else if ( (*it_beg).at<int>(1,0) > (*it_nleft_nright_beg).queryIdx )
			it_nleft_nright_beg++;
		else
			it_beg = result.erase(it_beg);
	}
	
	return result;
}

void
StereoCourseTracker::print_paired_keypoints(std::vector<Mat> vec, size_t i)
{
	for (auto el : vec)
	{
		std::cout << "..................." << std::endl;
		std::cout << key_points[i - 1].first[el.at<int>(0,0)].pt << " "
				  << key_points[i - 1].second[el.at<int>(0,1)].pt << std::endl;
		std::cout << key_points[i].first[el.at<int>(1,0)].pt << " "
				  << key_points[i].second[el.at<int>(1,1)].pt << std::endl;
	}
	std::cout << "------" << vec.size() << "-------" << std::endl;
}

void
draw_matches(Mat img_1, std::vector<KeyPoint> kps_1,
			 Mat img_2, std::vector<KeyPoint> kps_2,
			 std::vector<DMatch> good_matches)
{
	Mat img_matches;
	drawMatches(img_1, kps_1, 
				img_2, kps_2, 
				good_matches, img_matches, Scalar::all(-1),
				Scalar::all(-1), std::vector<char>(),
				DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
	// Show detected matches
	imshow("Good Matches", img_matches );
	waitKey();
}

std::vector<std::vector<cv::Point2f>>
StereoCourseTracker::get_key_points_by_index(std::vector<cv::Mat> vec, size_t i)
{

	std::vector<std::vector<cv::Point2f>> mtrx_of_kps(4);

	for (auto el : vec)
	{
		Point2f curr_l_p = key_points[i - 1].first[el.at<int>(0,0)].pt;
		Point2f curr_r_p = key_points[i - 1].first[el.at<int>(0,1)].pt;
		Point2f next_l_p = key_points[i].first[el.at<int>(1,0)].pt;
		Point2f next_r_p = key_points[i].first[el.at<int>(1,1)].pt;

		mtrx_of_kps[0].push_back(curr_l_p);
		mtrx_of_kps[1].push_back(curr_r_p);
		mtrx_of_kps[2].push_back(next_l_p);
		mtrx_of_kps[3].push_back(next_r_p);
	}
	return mtrx_of_kps;
}

template<typename T>
void print_vec(std::vector<T> vec){
    for (auto &&i : vec)
    {
        std::cout << i << std::endl;
    }
    // std::cout << std::endl;
}

cv::Mat
homogenues_to_xyz(cv::Mat points)
{
	cv::Mat xyz = cv::Mat(points, cv::Rect(0,0,points.cols,3));
    cv::Mat scale = cv::Mat(points, cv::Rect(0,3,points.cols,1));
    cv::Mat a, b;

    cv::vconcat(scale, scale, a);
    cv::vconcat(a, scale, b);
    // std::cout << "tmp p :" << std::endl << b << std::endl;

	return xyz/b;
}

std::vector<cv::Mat>
StereoCourseTracker::triangulate_matched_points(std::vector<Mat> indexes, size_t i,
									   CalibReader calib_data)
{
	std::vector<cv::Mat> real_world_points;
	cv::Mat trng_cur_rw_p;
	cv::Mat trng_nxt_rw_p;
	std::vector<cv::Mat> input_curr_p(2);
	std::vector<cv::Mat> input_next_p(2);

	std::vector<std::vector<cv::Point2f>> kps_by_index = get_key_points_by_index(indexes, i);

	for (size_t i = 0; i < 2; i++){
		for (size_t j = 0; j < 2; j++){
			std::vector<cv::Point2f> tmp_ps = kps_by_index[i*2 + j];
			if (tmp_ps.empty()){
				return real_world_points;
			}
			cv::Mat ps = cv::Mat(tmp_ps).reshape(1,2);
			if (i == 0)
				input_curr_p[j].push_back(ps);
			else
				input_next_p[j].push_back(ps);
		}
	}
	// undistortPoints(c_n_left_p, new_c_n_left_p, left_cam_mtrx, dist);
	// undistortPoints(c_n_right_p, new_c_n_right_p, right_cam_mtrx, dist);

	cv::Mat proj_m_l_cam = cv::Mat(calib_data.calib_cam_data[0].P_rect_xx(), true).reshape(1,3);
	cv::Mat proj_m_r_cam = cv::Mat(calib_data.calib_cam_data[1].P_rect_xx(), true).reshape(1,3);
	std::vector<cv::Mat> proj_m{proj_m_l_cam, proj_m_r_cam};

	cv::triangulatePoints(proj_m_l_cam, proj_m_r_cam, input_curr_p[0], input_curr_p[1], trng_cur_rw_p);
	cv::triangulatePoints(proj_m_l_cam, proj_m_r_cam, input_next_p[0], input_next_p[1], trng_nxt_rw_p);

	// convert from homogenues coords to x, y, z
	cv::Mat curr_real_xyz, next_real_xyz;
	curr_real_xyz = homogenues_to_xyz(trng_cur_rw_p);
	next_real_xyz = homogenues_to_xyz(trng_nxt_rw_p);
	curr_real_xyz.convertTo(curr_real_xyz, CV_64F);
	next_real_xyz.convertTo(next_real_xyz, CV_64F);

	real_world_points.push_back(curr_real_xyz);
	real_world_points.push_back(next_real_xyz);

	// cv::sfm::triangulatePoints(input_curr_p, proj_m, trng_cur_rw_p);
	// cv::sfm::triangulatePoints(input_next_p, proj_m, trng_nxt_rw_p);
	// real_world_points.push_back(trng_cur_rw_p);
	// real_world_points.push_back(trng_nxt_rw_p);

	return real_world_points;
}

cv::Mat
StereoCourseTracker::diff_between_points(std::vector<cv::Mat> c_and_p_points)
{
	return c_and_p_points[1] - c_and_p_points[0];
}

bool
check_by_eps(cv::Mat p, const float eps)
{
	if (p.at<double>(0, 0) < eps &&
		p.at<double>(1, 0) < eps &&
		p.at<double>(2, 0) < eps &&
		p.at<double>(0, 0) > -eps &&
		p.at<double>(1, 0) > -eps &&
		p.at<double>(2, 0) > -eps
	)
		return true;
}

std::vector<cv::Mat>
remove_outliers_by_eps(std::vector<cv::Mat> points, const float eps = 5.0)
{
	size_t sz = points.size();
	std::vector<cv::Mat> res(sz);


	for (size_t i = 0; i < points[0].cols; i++)
	{
		if (check_by_eps(points[0].col(i), eps) &&
			check_by_eps(points[1].col(i), eps))
		{
			// std::cout << "col = " << points.col(i) << std::endl;
			res[0].push_back(points[0].col(i).t());
			res[1].push_back(points[1].col(i).t());
		}
	}

	res[0] = res[0].t();
	res[1] = res[1].t();

	return res;
}

cv::Point3f
StereoCourseTracker::get_pose(std::vector<cv::Mat> triang_p, cv::Mat &t_prev)
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

    // std::cout << "R: \n" << R << std::endl;
    // std::cout << "t: \n" << t << std::endl;

	if (!R.empty())
	{
		pose = (R*(t_prev - t).t());
		t_prev = t;
	}

	return cv::Point3f(pose);
}

void
StereoCourseTracker::test(TruePathReader truth_p)
{
	navigation_data.clear(); 
	navigation_data.push_back(cv::Point3f(0,0,0));
	size_t k = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 0.2);
	float coef = truth_p.poses.size()/2;
	for (auto &&el : truth_p.poses)
	{
		cv::Point3f ofs = cv::Point3f(dis(gen), dis(gen), std::pow(k,-1)*0.001);
		if (k != 0)
			if (k < coef)
				navigation_data.push_back(el+ofs+ofs*(5/(coef*2)));
			else
				navigation_data.push_back(el+ofs-ofs*(5/(coef*2)));
		k++;
	}
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

    // std::cout << "R: \n" << R << std::endl;
    // std::cout << "t: \n" << t << std::endl;

	if (!R.empty())
	{
		pose = (R*(p_prev - t).t());
		p_prev = pose.t();
	}
	// std::cout << "Pose " << pose << std::endl;

	return cv::Point3f(pose);
}

void
StereoCourseTracker::track_course(const size_t count_images, ImageReader reader,
								  CalibReader calib_data)
{
	cv::Mat prev_img_left, prev_img_right;

	cv::Mat p_prev = (cv::Mat_<float>(1,3)<< 0, 0, 0);
	p_prev.convertTo(p_prev, CV_64F);
	// navigation_data.push_back(cv::Point3f(p_prev));
	cv::Point3f pose;
	for (size_t i = 0; i < count_images; i++)
	{
		Mat curr_img_left, curr_img_right;

		reader.read_pair_image(curr_img_left, curr_img_right, i);
		StereoPointTracker tracker(curr_img_left, curr_img_right);

		tracker.detect_features();
		tracker.match_features();
		tracker.get_good_matches();

	#ifdef DEBUG_DRAW_ENABLE
		std::cout << "Image left right for pair :" << i << std::endl;
		draw_matches(curr_img_left, tracker.kps_l, curr_img_right,
					 tracker.kps_r, tracker.good_matches);
	#endif

		key_points.push_back(std::make_pair(tracker.kps_l,tracker.kps_r));

		if (i != 0)
		{
			auto pleft_cleft_match =
			StereoPointTracker::match_with_second_pair(prev_img_left,
													   curr_img_left,
													   key_points[i-1].first,
													   tracker.kps_l);
			// auto pright_cright_match =
			// StereoPointTracker::match_with_second_pair(prev_img_right,
			// 									curr_img_right,
			// 									key_points[i-1].second,
			// 									tracker.kps_r);
			good_matches.push_back(pleft_cleft_match);

		#ifdef DEBUG_DRAW_ENABLE
			std::cout << "Match curr image left - next image left:" << std::endl;
			draw_matches(prev_img_left, key_points[i-1].first, curr_img_left,
						 tracker.kps_l, pleft_cleft_match);
		#endif

		}
		good_matches.push_back(tracker.good_matches);

		if (i > 0)
		{
			std::vector<Mat> matched_p = match_paired_points();

			
		#ifdef DEBUG_LOG_ENABLE
			std::cout << "Count of mathed points :" << matched_p.size() << std::endl;
			std::cout << "resulted points for image " << i - 1 << std::endl;
			print_paired_keypoints(matched_p, i);
			std::cout << "resulted points for image " << i - 1 << std::endl;

			std::cout << "Count of math cur left-right :" << good_matches[0].size() << std::endl;
			std::cout << "Count of math cur left next left :" << good_matches[1].size() << std::endl;
			std::cout << "Count of math next left-right :" << good_matches[2].size() << std::endl;
			std::cout << "size good match = " << good_matches.size() << std::endl;
			std::cout << "size key_points = " << key_points.size() << std::endl;
		#endif
			
			std::vector<cv::Mat> triang_p = triangulate_matched_points(matched_p, i, calib_data);
			if (!triang_p.empty()){
				//clear data
			std::vector<cv::Mat> cleared_data = remove_outliers_by_eps(triang_p, 50);

			/*
			std::cout << "For " << i << " " << cleared_data[0].size() << std::endl;
			for(size_t i = 0; i < cleared_data[0].cols; i++){
				std::cout << "x1 :" << cleared_data[0].col(i).t() << " "
						  << "x2 :" << cleared_data[1].col(i).t() << std::endl;
			}
			*/
			// cv::Point3f pose = get_pose(triang_p, p_prev);

			// MSQ min_sq(cleared_data);
			// cv::Point3f pose = min_sq.get_pose(p_prev);
			cv::Mat p1, p2;
			p1 = convet_mat_to_mat_vec(cleared_data[0]);
   			p2 = convet_mat_to_mat_vec(cleared_data[1]);
			cv::Mat Rt = FindRigidTransform(p1, p2);
			pose = get_pose_from_r(Rt, p_prev);

			// remove outliers by 3 sigma rule
			// cv::Mat filtered_by_value = remove_outliers_by_eps(distance, 2.0);
			// StatisticalProcessing st_p(filtered_by_value);
			// st_p.prepare_data();
			// cv::Mat filtered_by_sigma = remove_outliers_n_sigma(filtered_by_value, 3*st_p.stddev);
			
			// cv::Scalar mean_f = st_p.prepare_data(filtered_by_sigma);
			// std::cout << "mean v : " << mean_f << std::endl;
			}
			
			navigation_data.push_back(pose);
			good_matches.erase(good_matches.begin(), good_matches.begin()+2);
		}

		prev_img_left.release();
		prev_img_right.release();

		prev_img_left  = curr_img_left;
		prev_img_right = curr_img_right;
	}
}

#endif
