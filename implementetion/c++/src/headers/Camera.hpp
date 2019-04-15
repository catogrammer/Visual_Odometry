
class Camera {
public:
    Eigen::Vector3d cam_pose; //position of cameras in global coord
    Eigen::Vector3d normal; // cameras direction //view direction in global coord
    Eigen::Vector3d horizon; // right vector or x' in global coord
    Eigen::Vector3d vertical; // left vector or y' in global coord
    Eigen::MatrixXd features; // in local coordinate

    Camera(){}

    void read_property_cam(std::string path) {
        std::ifstream fin(path, std::ifstream::in);
        Eigen::MatrixXd tmp(3,3);
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                fin >> tmp(i,j);
            }
        }
        this->cam_pose << tmp.row(0).transpose();
        this->normal   << tmp.row(1).transpose();
        this->horizon  << tmp.row(2).transpose();
        fin.close();
    }

    void write_to_file(std::string path) {
        std::ofstream fout(path, std::ofstream::app);
        if(!fout.is_open()){
            std::cout << "data doesn't read" << '\n';
        }
        for (size_t i = 0; i < (size_t)this->features.cols(); i++) {
            fout << this->features(0, i) << ' ' << this->features(1, i) << ' ' << this->features(2, i) << '\n';
        }
        fout << '\n';

        fout.close();
    }

    void read_features(std::string path) {
        std::ifstream fin(path);
        if(!fin){
            std::cout << "data doesn't read" << '\n';
        }
        size_t count_features = 0;
        fin >> count_features;

        this->features = Eigen::MatrixXd(3,count_features);
        for (size_t i = 0; i < count_features; i++)
            for (size_t j = 0; j < 3; j++)
                fin >> this->features(j,i);

        fin.close();
    }

    Eigen::Vector3d projection_on_palne(Eigen::Vector3d normal, Eigen::Vector3d point, double D){
        return point - (((point.dot(normal)) + D) / (normal.dot(normal)))*normal;
    }

    void transform_to_homogen(double D) {
        size_t count_features =  this->features.cols();
        // Eigen::MatrixXd res(2, count_features);
        Eigen::MatrixXd A(3,2);
        A.col(0) = this->horizon;
        A.col(1) = this->vertical;
        for (size_t i = 0; i < count_features; i++) {
            Eigen::Vector3d b(projection_on_palne(this->normal, this->features.col(i), D));
            Eigen::Vector2d res = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
            this->features.col(i) = Eigen::Vector3d(res[0], res[1], 1);
            // res.col(i) = A.householderQr().solve(b);
        }
    }

};
