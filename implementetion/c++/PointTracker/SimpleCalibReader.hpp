#ifndef SIMPLECALIBREADER_HPP
#define SIMPLECALIBREADER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <ios>

#define CNT_CAMERAS 4
#define PROPERTIES_CNT 1

struct SimpleCalibData
{
    std::vector<float> data[PROPERTIES_CNT];

    #define FUNCTION(name, idx) std::vector<float> name##_xx() { return data[idx];}
    FUNCTION(P, 0)

    std::ostream& operator << (std::ostream &out)
    {
        for (size_t i = 0; i < PROPERTIES_CNT; i++){
            for (auto el : data[i])
                std::cout << el << ' ';
            std::cout << std::endl;
        }
            
        return out;
    }

};

class SimpleCalibReader
{
private:

    std::string read_data_from_file(std::string path);
public:
    SimpleCalibData calib_cam_data[CNT_CAMERAS];
    SimpleCalibReader(){};
    ~SimpleCalibReader(){};

    void load_calib_data(std::string path);
};

std::string
SimpleCalibReader::read_data_from_file(std::string path){
    try
    {
        std::ifstream ifs(path);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string content(buffer.str());
        ifs.close();
        return content;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }
    
}

void calc_matches(std::vector<std::string> &vect_matches,
                  const std::string& in, const std::string& re)
{
    std::smatch m;
    std::regex_search(in, m, std::regex(re));
    if(m.empty()) {
        // std::cout << "NO MATCH\n";
    } else {
        vect_matches.push_back(m[1]);
        calc_matches(vect_matches, m.suffix(),re);
    }
}


void
SimpleCalibReader::load_calib_data(std::string path)
{
    std::vector<std::string> vect_matches;
    std::string source = read_data_from_file(path);

    calc_matches(vect_matches, source, "P[0-9]: (.*)");
    size_t idx = 0;

    for (auto el : vect_matches)
    {
        std::istringstream iss(el);
        std::copy(std::istream_iterator<float>(iss),
                  std::istream_iterator<float>(),
                  std::back_inserter(calib_cam_data[(int)idx/PROPERTIES_CNT].data[idx%PROPERTIES_CNT]));
        idx++;
    }
}

#endif