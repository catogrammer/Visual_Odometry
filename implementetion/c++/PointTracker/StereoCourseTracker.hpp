#ifndef STEREOCOURSETRACKER_HPP
#define STEREOCOURSETRACKER_HPP

#include <utility>

#include "CourseTracker.hpp"
#include "StereoPointTracker.hpp"
#include "Transformation.hpp"

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

std::vector<cv::Mat>
StereoCourseTracker::triangulate_matched_points(std::vector<Mat> indexes, size_t i,
									   CalibReader calib_data)
{
	std::vector<cv::Mat> real_world_points;
	cv::Mat curr_real_world_points;
	cv::Mat next_real_world_points;
	std::vector<cv::Mat> input_curr_p(2);
	std::vector<cv::Mat> input_next_p(2);

	std::vector<std::vector<cv::Point2f>> kps_by_index = get_key_points_by_index(indexes, i);

	for (size_t i = 0; i < 2; i++){
		for (size_t j = 0; j < 2; j++){
			std::vector<cv::Point2f> tmp_ps = kps_by_index[i*2 + j];
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

	cv::sfm::triangulatePoints(input_curr_p, proj_m, curr_real_world_points);
	cv::sfm::triangulatePoints(input_next_p, proj_m, next_real_world_points);

	real_world_points.push_back(curr_real_world_points);
	real_world_points.push_back(next_real_world_points);

	return real_world_points;
}

cv::Mat
StereoCourseTracker::diff_between_points(std::vector<cv::Mat> c_and_p_points)
{
	return c_and_p_points[1] - c_and_p_points[0];
}

// cv::Mat
// remove_outliers_n_sigma(cv::Mat points, cv::Scalar sigma)
// {
// 	cv::Mat res;
// 	std::cout << "sigma = " << sigma << std::endl;

// 	for (size_t i = 0; i < points.rows; i++)
// 	{
		
// 		if (points.at<double>(i, 0) > -sigma[0] &&
// 			points.at<double>(i, 1) > -sigma[1] &&
// 			points.at<double>(i, 2) > -sigma[2] &&
// 			points.at<double>(i, 0) < sigma[0] &&
// 			points.at<double>(i, 1) < sigma[1] &&
// 			points.at<double>(i, 2) < sigma[2])
// 		{
// 			res.push_back(points.row(i));
// 		}
// 	}
// 	return res;
// }

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
		R = cv::Mat(Rt_mat, Rect(0,0,3,3));
		t = cv::Mat(Rt_mat, Rect(3,0,1,3)).t();
	}

	if (!R.empty())
	{
		pose = (R*(t_prev - t).t());
		t_prev = t;
	}

	return cv::Point3f(pose);
}

void
StereoCourseTracker::track_course(const size_t count_images, ImageReader reader,
								  CalibReader calib_data)
{
	cv::Mat prev_img_left, prev_img_right;

	cv::Mat t_prev = (cv::Mat_<float>(1,3)<< 0, 0, 0);
	t_prev.convertTo(t_prev, CV_64F);
	navigation_data.push_back(cv::Point3f(t_prev));

	for (size_t i = 0; i < count_images; i++)
	{
		Mat curr_img_left, curr_img_right;

		reader.read_pair_image(curr_img_left, curr_img_right, i);
		StereoPointTracker tracker(curr_img_left, curr_img_right);

		tracker.detect_features();
		tracker.match_features();
		tracker.get_good_matches();

	#ifdef DEBUG_DRAW_ENABLE
		std::cout << "Curr image left right:" << std::endl;
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
			std::cout << "Curr image left  next image left:" << std::endl;
			draw_matches(prev_img_left, key_points[i-1].first, curr_img_left,
						 tracker.kps_l, pleft_cleft_match);
		#endif

		}
		good_matches.push_back(tracker.good_matches);

	#ifdef DEBUG_LOG_ENABLE
		std::cout << "size good match = " << good_matches.size() << std::endl;
		std::cout << "size key_points = " << key_points.size() << std::endl;
	#endif

		if (i > 0)
		{
			std::vector<Mat> matched_p = match_paired_points();

			std::cout << "Count of mathed points :" << matched_p.size() << std::endl;
			std::cout << "Count of math cur left-right :" << good_matches[0].size() << std::endl;
			std::cout << "Count of math cur left next left :" << good_matches[1].size() << std::endl;
			std::cout << "Count of math next left-right :" << good_matches[2].size() << std::endl;


		#ifdef DEBUG_LOG_ENABLE
			// print_paired_keypoints(matched_p, i);
			std::cout << "resulted points for image " << i - 1 << std::endl;
		#endif
		
			std::vector<cv::Mat> triang_p = triangulate_matched_points(matched_p, i, calib_data);
			//clear data
			std::vector<cv::Mat> cleared_data = remove_outliers_by_eps(triang_p, 15);

			std::cout << "Cleared size for image: " << i << std::endl;
			std::cout << cleared_data[0].size() << " "
			<< cleared_data[1].size() << std::endl;
			std::cout << "Cleared size for image: " << i << std::endl;

			// get pose
			cv::Point3f pose = get_pose(cleared_data, t_prev);

			// float arr[][] = {{1., 4., 5.},{2., 3., 1.},{4., 1., 3.},{0., 3., 3.}};
			// cv::Mat test_m(3, 4, CV_64F, &arr);
			// cv::Mat test_m2 = 

			// std::cout << "TEST" << std::endl;
			// cv::Mat distance = diff_between_points(triang_p);

			// remove outliers by 3 sigma rule
			// cv::Mat filtered_by_value = remove_outliers_by_eps(distance, 2.0);
			// StatisticalProcessing st_p(filtered_by_value);
			// st_p.prepare_data();
			// cv::Mat filtered_by_sigma = remove_outliers_n_sigma(filtered_by_value, 3*st_p.stddev);

			// std::cout << "Distance beg: " << i << std::endl;
			// std::cout << distance << std::endl;
			// std::cout << "Distance end: " << i << std::endl;

			// std::cout << "Cleared distance beg: " << i << std::endl;
			// std::cout << filtered_by_value << std::endl;
			// std::cout << "Cleared distance end: " << i << std::endl;
			
			// cv::Scalar mean_f = st_p.prepare_data(filtered_by_sigma);
			// std::cout << "mean v : " << mean_f << std::endl;
			
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
