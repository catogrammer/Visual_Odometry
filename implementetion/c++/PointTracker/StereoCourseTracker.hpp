#ifndef STEREOCOURSETRACKER_HPP
#define STEREOCOURSETRACKER_HPP

#include <utility>

#include "CourseTracker.hpp"
#include "StereoPointTracker.hpp"

#include <opencv2/sfm/triangulation.hpp>
#include "opencv2/calib3d.hpp"
#include "StatisticalProcessing.hpp"


class StereoCourseTracker : protected CourseTracker {
private:

public:
	std::vector<std::pair<std::vector<KeyPoint>,
						  std::vector<KeyPoint>
						  >
						  		> key_points;
	std::vector<std::vector<DMatch>> good_matches;

	StereoCourseTracker() : CourseTracker(){}
	~StereoCourseTracker(){}

	void track_course(const size_t count_images,
					  ImageReader reader, CalibReader calib_data);
	std::vector<Mat> get_indexes();
	std::vector<std::vector<cv::Point2f>> get_key_points_by_index(std::vector<cv::Mat> vec, size_t i);
	void print_paired_keypoints(std::vector<Mat> vec, size_t i);
	std::vector<cv::Mat> get_result_points(std::vector<Mat> indexes, size_t i,
										   CalibReader calib_data);
};

void print_vector_dmatch(std::vector<std::vector<DMatch>> vec_dmatch) {
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
StereoCourseTracker::get_indexes()
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
		else if ( (*it_cletf_cright_beg).queryIdx < (*it_cleft_nleft_beg).queryIdx )
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
	//-- Show detected matches
	imshow("Good Matches", img_matches );
	waitKey();
}

std::vector<std::vector<cv::Point2f>>
StereoCourseTracker::get_key_points_by_index(std::vector<cv::Mat> vec, size_t i){

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
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

std::vector<cv::Mat>
StereoCourseTracker::get_result_points(std::vector<Mat> indexes, size_t i,
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

	// std::cout << "DSLKFJL:"  << next_real_world_points << std::endl;

	return real_world_points;
}

void
StereoCourseTracker::track_course(const size_t count_images,
								  ImageReader reader,
								  CalibReader calib_data)
{
	Mat prev_img_left, prev_img_right;
	std::vector<DMatch> prev_l_r_match;

	for (size_t i = 0; i < count_images; i++)
	{
		Mat curr_img_left, curr_img_right;

		reader.read_pair_image(curr_img_left, curr_img_right, i);
		StereoPointTracker tracker(curr_img_left, curr_img_right);

		tracker.detect_features();
		tracker.match_features();
		tracker.get_good_matches();

		// std::cout << "Curr image left right:" << std::endl;
		// draw_matches(curr_img_left, tracker.kps_l, curr_img_right,
		// 			 tracker.kps_r, tracker.good_matches);

		key_points.push_back(std::make_pair(tracker.kps_l,tracker.kps_r));

		if (i != 0)
		{
			auto pleft_cleft_match =
			StereoPointTracker::match__features(prev_img_left,
												curr_img_left,
												key_points[i-1].first,
												tracker.kps_l);
			// auto pright_cright_match =
			// StereoPointTracker::match__features(prev_img_right,
			// 									curr_img_right,
			// 									key_points[i-1].second,
			// 									tracker.kps_r);
			good_matches.push_back(pleft_cleft_match);
			// std::cout << "Curr image left  next image left:" << std::endl;
			// draw_matches(prev_img_left, key_points[i-1].first, curr_img_left,
			// 				tracker.kps_l, pleft_cleft_match);
		}
		good_matches.push_back(tracker.good_matches);

		std::cout << "size good match = " << good_matches.size() << std::endl;
		std::cout << "size key_points = " << key_points.size() << std::endl;

		if (i > 0)
		{
			std::vector<Mat> tmp = get_indexes();

			// print_paired_keypoints(tmp, i);
			std::cout << "resulted points for image " << i - 1 << std::endl;
			std::vector<cv::Mat> res_p = get_result_points(tmp, i, calib_data);
			StatisticalProcessing st_p(res_p);
			cv::Mat ffffff = st_p.prepare_data();

			std::cout << "fff: \n" << ffffff << std::endl;

			good_matches.erase(good_matches.begin(), good_matches.begin()+2);
		}

		prev_img_left.release();
		prev_img_right.release();

		prev_img_left  = curr_img_left;
		prev_img_right = curr_img_right;
	}
}

#endif
