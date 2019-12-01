#ifndef STEREOCOURSETRACKER_HPP
#define STEREOCOURSETRACKER_HPP

#include "CourseTracker.hpp"
#include "StereoPointTracker.hpp"
#include <utility>

// using namespace cv;

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
	void print_indexies();
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

void
StereoCourseTracker::print_indexies()
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
		{
			it_cletf_cright_beg++;
		}
		else
		{
			it_cleft_nleft_beg++;
		}
	}
	// for (auto mat : result)
	// 	std::cout << "mat = \n" << mat << std::endl;

	auto it_beg = result.begin();
	auto it_end = result.end();

	for (; it_beg != it_end; )
	{
		if ((*it_beg).at<int>(1,0) == (*it_nleft_nright_beg).queryIdx )
		{
			// std::cout << "value = \n" << (*it_beg).at<int>(1,0) << std::endl;
			(*it_beg).at<int>(1,1) = (*it_nleft_nright_beg).trainIdx;

			it_beg++;
		}
		else if ( (*it_beg).at<int>(1,0) > (*it_nleft_nright_beg).queryIdx )
		{
			it_nleft_nright_beg++;
		}
		else
		{
			it_beg++;
		}
	}

	for (auto mat : result)
		std::cout << "mat = \n" << mat << std::endl;

	std::cout << "**********************" << std::endl;
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

		key_points.push_back(std::make_pair(tracker.kps_l,tracker.kps_r));

		if (i != 0)
		{
			auto pleft_cleft_match =
			StereoPointTracker::match__features(prev_img_left,
												curr_img_left,
												key_points[i-1].first,
												tracker.kps_l);
			auto pright_cright_match =
			StereoPointTracker::match__features(prev_img_right,
												curr_img_right,
												key_points[i-1].second,
												tracker.kps_r);
			good_matches.push_back(pleft_cleft_match);
		}
		good_matches.push_back(tracker.good_matches);

		if (i > 0) print_indexies();
		
		prev_img_left.release();
		prev_img_right.release();

		prev_img_left  = curr_img_left;
		prev_img_right = curr_img_right;

		// std::vector<Matx22f> tmp_kps = tracker.get_good_coordinate();
		// // key_points.push_back(tracker.get_result_point_positions());
		// Point3f l_cam_p(-0.27, 0, 1);
		// Point3f r_cam_p(0.27, 0, 1);
		// std::vector<Point3f> tmp = tracker.get_result_point_positions(l_cam_p, r_cam_p);
		// for (auto el : tmp)
		// {
		//     std::cout << '[' << el.x << ',' << el.y << ' ' << el.z << ']' << std::endl;
		// }
		// std::cout << "size features = " << tmp.size() << std::endl;
		// //save detected features

	}
}

#endif
