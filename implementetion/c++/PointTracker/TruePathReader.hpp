#ifndef TRUEPATHREADER_HPP
#define TRUEPATHREADER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "opencv2/core.hpp"


class TruePathReader
{
private:
    std::string path;
public:
    std::vector<cv::Point3f> poses;

    TruePathReader(std::string path) : path(path) {};
    ~TruePathReader(){};

    bool read_data(const size_t& cnt_frms);
};


bool
TruePathReader::read_data(const size_t& cnt_frms)
{
    std::ifstream in(path, std::ifstream::in);
    if (!in.is_open())
        return false;

    poses.push_back(cv::Point3f(0,0,0));
    for (size_t i = 0; i < cnt_frms; i++)
    {
        std::string line;
        std::getline(in, line);
        std::stringstream ssin(line);
        std::vector<float> oxt;
        for(std::string s; ssin >> s; )
            oxt.push_back(std::stod(s));

        // std::cout << "x y z : " << oxt[3] << " " << oxt[11] << " " << oxt[7] << std::endl;
        if (i > 0)
            poses.push_back(cv::Point3f(oxt[3], oxt[11], oxt[7]));

    }
    
    in.close();
    return true;
}


#endif