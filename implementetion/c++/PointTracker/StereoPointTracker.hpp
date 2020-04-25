#ifndef STEREOPOINTTRACKER_HPP
#define STEREOPOINTTRACKER_HPP

#include "PointTracker.hpp"

#define RATIO_THRESH 0.73f

// template <class Detector, class Descriptor, class Matcher>
class StereoPointTracker : public PointTracker {
private:
public:
	std::vector<KeyPoint>				kps_l, kps_r;

	std::vector<std::vector<DMatch>>	knn_matches;
	std::vector<DMatch>	good_matches;

	std::vector<Matx22f> indesies_matched_points;

	Mat descriptors_l, descriptors_r;
	Mat image_l, image_r;

	StereoPointTracker(Mat image_l,
					   Mat image_r)
		:	PointTracker(),
			image_l(image_l),
			image_r(image_r){}

	void detect_features();
	void match_features();
	void get_good_matches();
	void get_matched_features();
	static std::vector<DMatch> match_with_second_pair(Mat image_1,
											Mat image_2,
											std::vector<KeyPoint> kps_1, 
											std::vector<KeyPoint> kps_2);
};

// template <class Detector>
void
StereoPointTracker::detect_features(){
	Ptr<FeatureDetector> detector = ORB::create();

	detector->detect(image_l, kps_l);
	detector->detect(image_r, kps_r);
}

// template <class Descriptor, Matcher>
void
StereoPointTracker::match_features(){
	Ptr<FeatureDetector> computer = ORB::create();
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);

	computer->compute(image_l, kps_l, descriptors_l);
	computer->compute(image_r, kps_r, descriptors_r);

	if(descriptors_l.type()!= CV_32F) {
    	descriptors_l.convertTo(descriptors_l, CV_32F);
	}

	if(descriptors_r.type()!= CV_32F) {
		descriptors_r.convertTo(descriptors_r, CV_32F);
	}

	matcher->knnMatch(descriptors_l, descriptors_r, knn_matches, 2);
}

std::vector<DMatch>
StereoPointTracker::match_with_second_pair(Mat image_1,
									Mat image_2,
									std::vector<KeyPoint> kps_1, 
									std::vector<KeyPoint> kps_2)
{
	Mat descriptors_1, descriptors_2;
	std::vector<std::vector<DMatch>> knn_matches_;
	std::vector<DMatch>	good_matches_;

	Ptr<FeatureDetector> computer = ORB::create();
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);

	computer->compute(image_1, kps_1, descriptors_1);
	computer->compute(image_2, kps_2, descriptors_2);

	if(descriptors_1.type()!=CV_32F) {
    	descriptors_1.convertTo(descriptors_1, CV_32F);
	}

	if(descriptors_2.type()!=CV_32F) {
		descriptors_2.convertTo(descriptors_2, CV_32F);
	}

	matcher->knnMatch(descriptors_1, descriptors_2, knn_matches_, 2);

	for (size_t i = 0; i < knn_matches_.size(); i++)
	{
		if (knn_matches_[i][0].distance < 
			RATIO_THRESH * knn_matches_[i][1].distance)
		{
			good_matches_.push_back(knn_matches_[i][0]);
		}
	}

	return good_matches_;
}

void
StereoPointTracker::get_good_matches()
{
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < 
			RATIO_THRESH * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
}

void
StereoPointTracker::get_matched_features()
{
	// KeyPoints stroerd Point2f type
	for (auto el : good_matches)
	{
		std::cout << kps_l[el.queryIdx].pt << " "
				  << kps_r[el.trainIdx].pt << std::endl;
	}
}

#endif
