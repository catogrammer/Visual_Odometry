#ifndef MSQ_HPP
#define MSQ_HPP

#include <vector>
#include <iostream>

#include "opencv2/core.hpp"
// #include "opencv2/core/optim.hpp"

class MSQ
{
private:
    std::vector <cv::Mat> c_n_pnts;
public:
    MSQ(std::vector <cv::Mat> p): c_n_pnts(p){};
    ~MSQ(){};

    cv::Mat centr_mass(cv::Mat points);
    cv::Mat cross_cov_mat(cv::Mat mu_p, cv::Mat mu_x);
    cv::Mat find_quaternion();
    cv::Mat minimized_f(cv::Mat R, cv::Mat t) const;
    cv::Point3f get_pose();
    cv::Point3f get_pose(cv::Mat &prev_t);
};

cv::Mat
MSQ::centr_mass(cv::Mat points)
{
    cv::Mat mean_(1, 3, CV_32F);
    for (size_t i = 0; i < 3; i++)
        mean_.at<float>(0, i) = cv::mean(points.row(i))[0];

    return mean_;
}

cv::Mat
MSQ::cross_cov_mat(cv::Mat mu_p, cv::Mat mu_x)
{
    cv::Mat cc_mat, mu_px;
    cv::Mat accum = cv::Mat::zeros(3,3, CV_64F);
    size_t cc_s = c_n_pnts[0].cols;

    for (size_t i = 0; i < cc_s; i++)
        accum = accum + c_n_pnts[0].col(i)*c_n_pnts[1].col(i).t();
    
    accum.convertTo(accum, CV_32F);
    mu_px = mu_p.t()*mu_x;
    cc_mat = accum/cc_s - mu_px;

    std::cout << "cross cov mat :\n" << cc_mat << std::endl;
    cv::Mat minimized_f();

    return cc_mat;
}

cv::Mat
get_R_by_quat(cv::Mat quart)
{
    cv::Mat R;
    float a = quart.at<float>(0,0);
    float b = quart.at<float>(0,1);
    float c = quart.at<float>(0,2);
    float d = quart.at<float>(0,3);

    // std::cout << "abcd is : " << a << " " << b << " " << c << " " << d << std::endl;

    float tmp_r[3][3] = {{(a*a+b*b-c*c-d*d),
                          2*(b*c-a*d),
                          2*(b*d+a*c)},
                         {2*(b*c+a*d),
                          (a*a+c*c-b*b-d*d),
                          2*(c*d-a*b)},
                         {2*(b*d-a*c),
                          2*(c*d+a*b),
                          (a*a+d*d-b*b-c*c)}
                        };
    
    R = cv::Mat(3,3, CV_32F, &tmp_r);

    return R.clone();
}

// cv::Mat
// MSQ::find_quaternion()
// {

//     return E;
// }

// cv::Mat
// MSQ::minimized_f(cv::Mat R, cv::Mat t) const
// {
//     cv::Mat accum = cv::Mat::zeros(1,3, CV_64F);
//     R.convertTo(R, CV_64F);
//     size_t cc_s = c_n_pnts[0].cols;

//     for (size_t i = 0; i < cc_s; i++){
//         cv::Mat powed;
//         cv::Mat mat_v = c_n_pnts[1].col(i) - R*c_n_pnts[0].col(i) - t;
//         cv::pow(mat_v, 2, powed);
//         accum = accum + powed;
//     }
        
//     accum.convertTo(accum, CV_32F);

//     return accum/cc_s;
// }

cv::Point3f
MSQ::get_pose(cv::Mat &prev_t)
{   
    prev_t.convertTo(prev_t, CV_32F);
    cv::Mat pose = prev_t;
    cv::Mat mu_p = centr_mass(c_n_pnts[0]);
    cv::Mat mu_x = centr_mass(c_n_pnts[1]);
    cv::Mat cc_m = cross_cov_mat(mu_p, mu_x);
    
    
    cv::Mat tr_px = cv::Mat(1,1,CV_32F,cv::trace(cc_m)[0]);
    cv::Mat delta_mat = cc_m - cc_m.t();

    std::cout << "Centers mass:\n"
              << "mu_p : " << mu_p << std::endl
              << "mu_x : " <<  mu_x << std::endl;
    std::cout << "Delta mat :\n" << delta_mat << std::endl;

    float delta[3] = {delta_mat.at<float>(1,2),
                      delta_mat.at<float>(2,0),
                      delta_mat.at<float>(0,1)};

    // std::cout << "delta : " << std::endl;
    // for (size_t i = 0; i < 3; i++)
    //     std::cout << delta[i] << " ";
    
    cv::Mat delt_v = cv::Mat(1,3, CV_32F, &delta);
    std::cout << "Delta v :\n" << delt_v << std::endl;

    cv::Mat Q_px, tmp;
    cv::Mat tr_I = tr_px.at<float>(0,0)*cv::Mat::eye(3,3, CV_32F);

    cv::vconcat(tr_px, delt_v.t(), Q_px);
    cv::vconcat(delt_v, cc_m + cc_m.t() - tr_I, tmp);
    cv::hconcat(Q_px, tmp, Q_px);

    cv::Mat E, V;

    cv::eigen(Q_px, E, V);
    std::cout << "Q_px:\n" << Q_px << std::endl;
    std::cout << "Eigen v:\n" << E << std::endl;

    // cv::Mat E = find_quaternion();
    cv::Mat R_ = get_R_by_quat(E);
    std::cout << "R :\n" << R_ << std::endl;
    // std::cout << "mu :\n" << mu_x.type() << " " << mu_p << std::endl;
    cv::Mat t_ = mu_x - (R_*mu_p.t()).t();

    // cv::Ptr<cv::MinProblemSolver::Function> min_f;
    // cv::Ptr<cv::ConjGradSolver> solver = cv::ConjGradSolver::create(min_f);
    // cv::Mat t = solver->minimize();
    std::cout << "t : " << t_ << "\n" << prev_t << std::endl;
    pose = R_*(prev_t - t_).t();
    prev_t = t_;
    return cv::Point3f(pose);
}

#endif