#ifndef STEREOCOURSETRACKER_HPP
#define STEREOCOURSETRACKER_HPP

#include "CourseTracker.hpp"
#include "StereoPointTracker.hpp"
#include <utility>

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

	void track_course(const size_t count_images, DataReader reader);
	std::vector<Mat> get_indexies();
	void print_paired_keypoints(std::vector<Mat> vec, size_t i);
	std::vector<Point3f> get_result_points(std::vector<Mat> vec, size_t i);
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
StereoCourseTracker::get_indexies()
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
		std::cout << "-------------" << std::endl;
	}
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


// cv::Point3f
// get_point_position(Point3f point_curr_l, Point3f point_next_l,
//            Point3f point_curr_r, Point3f point_next_r)
// {
//     cv::Point3f across_point;

//     cv::Point3f p_a = point_next_l - point_curr_l;
//     cv::Point3f p_b = point_next_r - point_curr_r;
//     double t = (point_curr_r.x - point_curr_l.x)/(p_a.x - p_b.x);

//     across_point = p_a*t + point_curr_l;

//     return across_point;
// }

cv::Point3f
get_point_position(cv::Mat c_n_left_p, cv::Mat c_n_right_p)
{
    cv::Point3f across_point;

    cv::Mat p_a = c_n_left_p.row(1) - c_n_left_p.row(0);
    cv::Mat p_b = c_n_right_p.row(1) - c_n_right_p.row(0);
    float t = (c_n_right_p.at<float>(0,0) - c_n_left_p.at<float>(0,0))/(p_a.at<float>(0) - p_b.at<float>(0));

	cv::Mat tmp = p_a*t + c_n_left_p.row(0);
    across_point = Point3f(tmp.at<float>(0), tmp.at<float>(1), tmp.at<float>(2));
	
    return across_point;
}

std::vector<Point3f>
StereoCourseTracker::get_result_points(std::vector<Mat> vec, size_t i)
{
	std::vector<Point3f> result_points;

	for (auto el : vec)
	{
		//transform to global coords.
		cv::Mat new_c_n_left_p;
		cv::Mat new_c_n_right_p;
		cv::Mat left_cam_mtrx;
		cv::Mat right_cam_mtrx;
		cv::Mat dist;

		std::vector<Point2f> c_n_left_p = {key_points[i - 1].first[el.at<int>(0,0)].pt,
										   key_points[i].first[el.at<int>(1,0)].pt};
		std::vector<Point2f> c_n_right_p = {key_points[i - 1].second[el.at<int>(0,1)].pt,
											key_points[i].second[el.at<int>(1,1)].pt};

		float cx = 0.54;
		float cy = 0.0;
		float fx = 1;
		float fy = 1;

		float l_data[] = {fx,  0,  0, 
						  0, fy, cy, 
						  0,  0,  1};

		float r_data[] = {fx,  0, cx, 
						  0, fy, cy, 
						  0,  0,  1};

		left_cam_mtrx = cv::Mat(3, 3, CV_32F, l_data);
		right_cam_mtrx = cv::Mat(3, 3, CV_32F, r_data);
		dist = cv::Mat::zeros(1, 5, CV_32F);
		
		undistortPoints(c_n_left_p, new_c_n_left_p, left_cam_mtrx, dist);
		undistortPoints(c_n_right_p, new_c_n_right_p, right_cam_mtrx, dist);


		std::cout << new_c_n_left_p  << std::endl
				  << new_c_n_right_p << std::endl
				//   << "-----------------" 
				  << std::endl;
		// // get coord result point
		cv::Point3f result_p = get_point_position(new_c_n_left_p, new_c_n_right_p);
		
		std::cout << "Res point: " << result_p << std::endl
				  << "-----------------" 
				  << std::endl;
		
		result_points.push_back(result_p);
	}
	return result_points;
}


void
StereoCourseTracker::track_course(const size_t count_images, DataReader reader)
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
		// draw_matches(curr_img_left, tracker.kps_l, curr_img_right, tracker.kps_r, tracker.good_matches);

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
			// draw_matches(prev_img_left, key_points[i-1].first, curr_img_left, tracker.kps_l, pleft_cleft_match);
		}
		good_matches.push_back(tracker.good_matches);

		std::cout << "size good match = " << good_matches.size() << std::endl;
		std::cout << "size key_points = " << key_points.size() << std::endl;

		if (i > 0)
		{
			std::vector<Mat> tmp = get_indexies();

			// print_paired_keypoints(tmp, i);
			get_result_points(tmp, i);

			good_matches.erase(good_matches.begin(), good_matches.begin()+2);
		}

		prev_img_left.release();
		prev_img_right.release();

		prev_img_left  = curr_img_left;
		prev_img_right = curr_img_right;
	}
}

#endif
