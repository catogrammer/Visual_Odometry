#include "headers/EssentMtrx.hpp"

Eigen::MatrixXd EsssentialMatrix::read_features(std::string path) {
    std::ifstream fin(path);
    if(!fin){
        std::cout << "data doesn't read" << '\n';
    }
    size_t count_features = 0;
    fin >> count_features;

    Eigen::MatrixXd features = Eigen::MatrixXd(3,count_features);
    for (size_t i = 0; i < count_features; i++)
        for (size_t j = 0; j < 3; j++)
            fin >> features(j,i);

    fin.close();
    return features;
}

// Eigen::MatrixXd coord_in_cam_space(const Eigen::MatrixXd& features, const Eigen::Vector3d& pose) {
//     size_t count_features = features.cols();
//     Eigen::MatrixXd res = Eigen::MatrixXd(3, count_features);
//     for (size_t i = 0; i < count_features; i++)
//             res.col(i) = features.col(i) - pose;
//     return res;
// }


void EsssentialMatrix::tranform_features_into_coord_cam() {
    this->camera_1 = new Camera();
    this->camera_2 = new Camera();
    camera_1->read_property_cam("../input_data/def_cam_1_data.txt");
    camera_2->read_property_cam("../input_data/def_cam_2_data.txt");

    // normalisation of coordinates
    camera_1->normal    = camera_1->normal.normalized(); // y
    camera_1->horizon   = camera_1->horizon.normalized(); // x
    camera_2->normal    = camera_2->normal.normalized(); //loc in cam pose coord
    camera_2->horizon   = camera_2->horizon.normalized(); //loc in cam pose coord
    // find y'
    camera_1->vertical << camera_1->horizon.cross(camera_1->normal); // == x * n
    camera_2->vertical << camera_2->horizon.cross(camera_2->normal);

    // camera_1.features = read_local_coord("../input_data/homogen_features.txt");
    Eigen::MatrixXd orig_feat = read_features("../input_data/global_coords.txt");
    std::cout << "*****************features - orig***********************" << '\n';
    std::cout << orig_feat << '\n';
    std::cout << "****************************************************" << '\n';
    std::cout << "cam 1 pose : " << camera_1->cam_pose.transpose() << '\n';
    std::cout << "cam 1 horizon : " << camera_1->horizon.transpose() << '\n';
    std::cout << "cam 1 normal : " << camera_1->normal.transpose() << '\n';

    //
    // camera_1->features = coord_in_cam_space(orig_feat.array(), camera_1->cam_pose.array()); //(row,column)
    // camera_2->features = coord_in_cam_space(orig_feat.array(), camera_2->cam_pose.array());; //(row,column)
    camera_1->features = orig_feat;
    camera_2->features = orig_feat;
}

void EsssentialMatrix::get_homogenues_coordinate() {

    // std::cout << "global features : \n" << camera_1->features << '\n';
    // std::cout << "Cam data : "
    //           << "\npose : " << camera_1->cam_pose.transpose()
    //           << "\nnormal : "   << camera_1->normal.transpose()
    //           << "\nhorizon : "  << camera_1->horizon.transpose()
    //           << "\nvertical : " << camera_1->vertical.transpose() << '\n';
    // std::cout << "local features : \n" << camera_1->features  << '\n';

    double D_1 = (-1)*(camera_1->normal.dot(camera_1->cam_pose));
    double D_2 = (-1)*(camera_2->normal.dot(camera_2->cam_pose));

    camera_1->coord_in_cam_space(D_1);
    camera_2->coord_in_cam_space(D_2);

    std::cout << "*****************features - 1***********************" << '\n';
    std::cout << camera_1->features << '\n';
    std::cout << "****************************************************" << '\n';

    std::cout << "*****************features - 2***********************" << '\n';
    std::cout << camera_2->features << '\n';
    std::cout << "****************************************************" << '\n';

    std::string path = "../input_data/features.txt";
    std::ofstream fout(path, std::ofstream::out);
    fout << camera_1->features.cols() << '\n';
    fout.close();
    //
    camera_1->write_to_file(path);
    camera_2->write_to_file(path);

}

double* EsssentialMatrix::calc_polynom(double x[3], double _x[3]) {
    int count = 0;
    double *equation = new double[9];
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            equation[count++] = x[j]*_x[i];
        }
    }
    return equation;
}

void EsssentialMatrix::calc_UV_S__R_t_x() {
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(EssentMtrx, Eigen::ComputeThinU | Eigen::ComputeThinV);
    std::cout << "singular values are:" << std::endl << svd.singularValues() << std::endl;
    std::cout << "U matrix: \n" << svd.matrixU() << std::endl;
    std::cout << "V matrix:" << std::endl << svd.matrixV() << std::endl;
    /*
        [t]_x = U * W * S * U^T   S - singular values
        R = U * W^-1 * V^T
        diag(S) = [s, s, 0]
        W^-1 = W^T=[[0, 1, 0], [-1, 0, 0], [0, 0, 1]]
    */

    Eigen::Matrix3d W_T, W, R, S, t_x;
    W_T << 0, 1, 0,
          -1,  0, 0,
           0,  0, 1;
    W <<   0,  -1, 0,
           1,  0, 0,
           0,  0, 1;
    S << svd.singularValues()[0], 0, 0,
         0, svd.singularValues()[1], 0,
         0, 					  0, 0;
    // std::cout << "Matrix W is transpose : \n" << W_T.transpose() << '\n';
    R = svd.matrixU() * W_T * svd.matrixV().transpose();
    std::cout << "Rotation matrix : \n" << R << '\n';
    t_x = svd.matrixU() * W * svd.matrixU().transpose();
    std::cout << "Matrix t_x : \n" << t_x << '\n';

    /*
        t_k = [[0, -t_z, t_y], [t_z, 0, -t_x], [-t_y, t_x, 0]]
    */

    std::cout << "Vector t_x : \n" << t_x(2,1) << ' '
              << t_x(2,0) << ' ' << t_x(1,0) << '\n';
}

void EsssentialMatrix::calculateMatrix(/* arguments */) {
    std::cout << "features size : " << features.size() << '\n';
    // вычисляем систему из наших точек
    for (auto el : features) {
        double *p = calc_polynom(el.first.coord, el.second.coord);
        system_lin_equat.push_back(p);
    }

    size_t size_m = features.size();
    // Переходим на Eigen
    Eigen::MatrixXd A(size_m, 9);
    for (size_t i = 0; i < size_m; i++) {
        for (size_t j = 0; j < 9; j++) {
            A(i,j) = system_lin_equat[i][j];
        }
    }

    std::cout << "Matrix A : \n" << A << '\n';
    // Находим ядро системы лин. ур.
    Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A);
    Eigen::MatrixXd x = lu_decomp.kernel();
    std::cout << "plurality vectors of kernel:\n" << lu_decomp.kernel() << std::endl;

    // std::cout << "!!!!!!!!!!!!!!checking!!!!!!!!!!!!!!!" << '\n';
    // std::cout << "A*x(0) = " << (A*x.col(0)).transpose() << '\n';
    // std::cout << "A*x(1) = " << (A*x.col(1)).transpose() << '\n';
    // std::cout << "A*x(2) = " << (A*x.col(2)).transpose() << '\n';
    // std::cout << "!!!!!!!!!!!!!!checking!!!!!!!!!!!!!!!" << '\n';

    std::string path= "../input_data/essential_matrix.txt";
    std::ofstream fout(path);
    if(!fout){
        std::cout << "file dosen't open" << '\n';
    }

    // для каждого вектора из ядра составляем матрицу Е
    // и вычисляем R и t_x из разожения SVD в calc_UV_S__R_t_x
    fout << x.cols() << '\n'; // number of vectors kernel
    for (size_t col_ker = 0; col_ker < (size_t)x.cols(); col_ker++) {
        std::cout << "*******-" << col_ker << "-*****************" << '\n';
        for(size_t i = 0, k = 0; i < 3; i++) {
            for(size_t j = 0; j < 3; j++, k++) {
                EssentMtrx(j, i) = x.col(col_ker)(k);
            }
        }

        fout << EssentMtrx << "\n\n";
        std::cout << "The solution is:\n" << EssentMtrx << std::endl;

        // раскладываем полученную EssentMtrx с помощью SVD на UVS  и находим R и t_x
        calc_UV_S__R_t_x();

        // std::cout << A*x.col(0) << '\n'
    }
    fout.close();

}

void EsssentialMatrix::simpleRead(std::string path) {
    std::ifstream fin(path);
    if(!fin){
        std::cout << "data doesn't read" << '\n';
    }
    std::vector<Coordinate> tmp1;
    std::vector<Coordinate> tmp2;
    double x, y, z;
    int size = 0;
    fin >> size;
    for (size_t i = 0; i < (size_t)size; i++) {
        fin >> x;
        fin >> y;
        fin >> z;
        // Coordinate c1 = Coordinate(x,y,z);
        tmp1.push_back( Coordinate(x,y,z) );
    }
    for (size_t i = 0; i < (size_t)size; i++) {
        fin >> x;
        fin >> y;
        fin >> z;
        // Coordinate c2 = Coordinate(x,y,z);
        tmp2.push_back( Coordinate(x,y,z) );
    }
    for (size_t i = 0; i < (size_t)size; i++)
        features.push_back(std::make_pair(tmp1[i], tmp2[i]));

    printRead();
    fin.close();
}

void EsssentialMatrix::printRead() {
    for (size_t i = 0; i < features.size(); i++) {
        features[i].first.printCoord();
        std::cout << '\t';
        features[i].second.printCoord();
        std::cout << '\n';
    }
}