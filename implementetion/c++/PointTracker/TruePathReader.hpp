#ifndef TRUEPATHREADER_HPP
#define TRUEPATHREADER_HPP


#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "opencv2/core.hpp"
#include "opencv2/core/softfloat.hpp"

#define LENGHT_FILE_NAME 10

class TruePathReader
{
private:
    std::string path;
public:
    std::vector<std::vector<double>> oxts;
    std::vector<cv::Point3f> poses;

    TruePathReader(std::string path) : path(path) {};
    ~TruePathReader(){};

    bool read_data(const size_t& cnt_imgs);
    bool convertOxtsToPose();
    std::string get_file_name(const size_t& index);
};

std::string
TruePathReader::get_file_name(const size_t& index)
{
	std::string file_name;
	std::string str_index = std::to_string(index);
	for (size_t i = 0; i < LENGHT_FILE_NAME - str_index.length(); i++)
		file_name += '0';

	return file_name + str_index + ".txt";
}

bool
TruePathReader::read_data(const size_t& cnt_imgs)
{
    for (size_t i = 0; i < cnt_imgs; i++)
    {
        std::string full_path = path + "data/" + get_file_name(i);
        // std::cout << full_path << std::endl;
        std::ifstream in(full_path, std::ifstream::in);

        if (!in.is_open())
            return false;

        std::string buff((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());
        std::stringstream ssin(buff);
        std::vector<double> oxt;
        for(std::string s; ssin >> s; )
            oxt.push_back(std::stod(s));
        
        oxts.push_back(oxt);        
        in.close();
    }

    return true;
}

cv::softdouble
latToScale(float lat)
{
    return cv::cos(cv::softdouble(lat) * cv::softdouble::pi() / cv::softdouble(180));
}

/**
 * converts lat/lon coordinates to mercator coordinates using mercator scale
**/
cv::Vec2d
latlonToMercator(double lat, double lon, double scale)
{
    cv::Vec2d merc;
    int er = 6378137;
    double mx = scale * lon * CV_PI * er / 180;
    double my = scale * er * cv::log( std::tan((90+lat) * CV_PI / 360) );
    merc << mx, my;
    return merc;
}

bool
TruePathReader::convertOxtsToPose()
{
    cv::softdouble scale = latToScale(oxts[0][0]);
    // std::cout << "scale : " << scale << std::endl;

    cv::Mat t_prev(1,3, CV_64F, cv::Scalar(0, 0, 0));
    poses.push_back(cv::Point3f(0.f, 0.f, 0.f));
    
    cv::Mat Tr_0_inv;
    cv::Mat zero_1 = (Mat_<double>(1, 4) << 0, 0, 0, 1);

    for (size_t i = 0; i < oxts.size(); i++)
    {
        cv::Mat pose;

        if (oxts[i].empty())
            continue;

        // translation vector

        cv::Vec2d tmp = latlonToMercator(oxts[i][0], oxts[i][1], scale);
        cv::Mat t = (Mat_<double>(1,3) << tmp[0], tmp[1], oxts[i][3]);
        // t << oxts[i][0], oxts[i][1] , oxts[i][3];

        // rotation matrix (OXTS RT3000 user manual, page 71/92)
        cv::softdouble rx(oxts[i][4]); // roll
        cv::softdouble ry(oxts[i][5]); // pitch
        cv::softdouble rz(oxts[i][6]); // heading 
        cv::Mat Rx, Ry, Rz, R;
        Rx = (Mat_<double>(3, 3) << 1, 0, 0, 0, cv::cos(rx), 
            -cv::sin(rx), 0, cv::sin(rx), cv::cos(rx));
        Ry = (Mat_<double>(3, 3) << cv::cos(ry), 0., cv::sin(ry), 0, 1, 0,
            -cv::sin(ry), 0, cv::cos(ry));
        Rz = (Mat_<double>(3, 3) << cv::cos(rz), -cv::sin(rz), 0, cv::sin(rz),
            cv::cos(rz), 0, 0, 0, 1);

        R  = Rz * Ry * Rx;

    #ifdef DEBUG_LOG_ENABLE
        std::cout << "R : \n" << R << std::endl;
        std::cout << "t : " << t << std::endl;
    #endif

        // normalize translation and rotation (start at 0/0/0)
        // if (Tr_0_inv.empty())
        // {
        //     cv::Mat tmp;
        //     hconcat(R, t.t(), tmp);
        //     vconcat(tmp, zero_1, Tr_0_inv);
        //     Tr_0_inv = Tr_0_inv.inv();
        // }
        // std::cout << "Tr_inv =  \n" << Tr_0_inv << std::endl;

        if (i > 0){
            pose = (R*(t_prev - t).t())/1000000;
            // std::cout << "Pose : " << pose << std::endl;
            poses.push_back(cv::Point3f(pose));
            t_prev = pose.t();
        }
        
        
        

    #ifdef DEBUG_LOG_ENABLE
        std::cout << "Pose : " << pose << std::endl;
    #endif   

    }
}



#endif