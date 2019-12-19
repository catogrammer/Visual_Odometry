#ifndef DATAREADER_HPP
#define DATAREADER_HPP

#include <string>
#include "opencv2/core.hpp"

using namespace cv;

#define LENGHT_IMAGE_NAME 10

class DataReader {
private:
	std::string path;
	std::string get_image_name(const size_t& index);

public:
	DataReader (std::string path) : path(path){};

	void read_pair_image(Mat& img_1, Mat& img_2, const size_t index_image);
};

std::string
DataReader::get_image_name(const size_t& index)
{
	std::string image_name;
	std::string str_index = std::to_string(index);
	for (size_t i = 0; i < LENGHT_IMAGE_NAME - str_index.length(); i++)
		image_name += '0';

	return image_name + str_index + ".png";
}

void
DataReader::read_pair_image(Mat& img_1, Mat& img_2, const size_t index_image)
{
	std::string image_name = get_image_name(index_image);
	std::string image_1_path = path + "image_02/" + "data/" + image_name;
	std::string image_2_path = path + "image_03/" + "data/" + image_name;

	std::cout << "path = " << image_1_path << std::endl;
	std::cout << "path = " << image_2_path << std::endl;

	img_1 = imread(image_1_path, IMREAD_GRAYSCALE);
	img_2 = imread(image_2_path, IMREAD_GRAYSCALE);

	if( !img_1.data || !img_2.data )
		std::cout << " --(!) Error reading images " << std::endl;
}

#endif
