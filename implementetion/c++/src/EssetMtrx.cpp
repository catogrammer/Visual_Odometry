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

void EsssentialMatrix::tranform_features_into_coord_cam() {
    this->camera_1 = new Camera();
    this->camera_2 = new Camera();

    camera_1->read_property_cam("../input_data/def_cam_1_data.txt");
    camera_2->read_property_cam("../input_data/def_cam_2_data.txt");

    Eigen::MatrixXd orig_features = read_features("../input_data/global_coords.txt");

    camera_1->features = orig_features;
    camera_2->features = orig_features;

    camera_1->transform_featutes();
    camera_2->transform_featutes();

}

void EsssentialMatrix::get_homogenues_coordinate() {

    std::cout << "*****************features - 1***********************" << '\n';
    std::cout << camera_1->features << '\n';

    std::cout << "*****************features - 2***********************" << '\n';
    std::cout << camera_2->features << '\n';


    camera_1->get_homogen_coord();
    camera_2->get_homogen_coord();


    std::string path = "../input_data/features.txt";
    std::ofstream fout(path, std::ofstream::out);
    fout << camera_1->features.cols() << '\n';
    fout.close();
    //
    camera_1->write_to_file(path);
    camera_2->write_to_file(path);

}

void EsssentialMatrix::calc_UV_S__R_t_x() {
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(EssentMtrx,
                                            Eigen::ComputeThinU
                                          | Eigen::ComputeThinV);
    std::cout << "singular values are:" << std::endl << svd.singularValues()
              << std::endl;
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
          -1, 0, 0,
           0, 0, 1;
    W <<   0,-1, 0,
           1, 0, 0,
           0, 0, 1;
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
              << t_x(0,2) << ' ' << t_x(1,0) << '\n';
}

Eigen::MatrixXd EsssentialMatrix::get_Ae_matrix() {
    size_t size_m = features.size();

    Eigen::MatrixXd A(size_m, 9);
    // loop for move on A matrix
    for (size_t i = 0; i < size_m; i++) {
            // loop for move on linear equation
        for (size_t k = 0, j = 0; k < 3; k++) {
            for (size_t r = 0; r < 3; r++, j++) {
                A(i, j) = features[i].first.coord[r]
                          * features[i].second.coord[k];
            }
        }
    }
    std::cout << "A matrix : \n" << A << '\n';
    return A;
}

Eigen::MatrixXd EsssentialMatrix::get_kernel_Ae_matrix(){
    // Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(get_Ae_matrix());
    // std::cout << "plurality vectors of kernel:\n" << lu_decomp.kernel()
    //           << std::endl;
    // return lu_decomp.kernel();

    // Eigen::EigenSolver<Eigen::MatrixXd> es(get_Ae_matrix());
    // std::cout << "The first eigenvectors:"
    //      << std::endl << es.eigenvectors().real() << std::endl;
    // return es.eigenvectors().real();
    Eigen::MatrixXd matr = get_Ae_matrix();
    Eigen::CompleteOrthogonalDecomposition<Eigen::MatrixXd> cod(matr);
    cod.compute(matr);
    std::cout << "rank : " << cod.rank() << "\n";
    // Find URV^T
    Eigen::MatrixXd V = cod.matrixZ().transpose();
    Eigen::MatrixXd Null_space = V.block(0, cod.rank(),V.rows(), V.cols() - cod.rank());
    std::cout << "The null space: \n" << Null_space << "\n" ;

    Eigen::MatrixXd P = cod.colsPermutation();
    Null_space = P * Null_space; // Unpermute the columns
    // The Null space:
    std::cout << "The null space: \n" << Null_space << "\n" ;
    return Null_space;
}

Eigen::VectorXd EsssentialMatrix::SVD_decompose() {
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(get_Ae_matrix(), Eigen::ComputeThinU | Eigen::ComputeThinV);
    std::cout << "Its right singular vectors are the columns of the thin V matrix:"
         << std::endl << svd.matrixV() << std::endl;

    return svd.matrixV().col(svd.matrixV().rows()-1);
}

void EsssentialMatrix::calculate_Ess_matrix(Eigen::VectorXd v) {
    for(size_t i = 0, k = 0; i < 3; i++)
        for(size_t j = 0; j < 3; j++, k++)
            EssentMtrx(j, i) = v(k); // mb make some lambda???
}

void EsssentialMatrix::calculate_from_svd() {
    calculate_Ess_matrix(SVD_decompose());
    std::cout << "The solution is:\n" << EssentMtrx << std::endl;
    calc_UV_S__R_t_x();
}

void EsssentialMatrix::calculate_from_ker() {
    std::cout << "features size : " << features.size() << '\n';
    Eigen::MatrixXd ker = get_kernel_Ae_matrix();


    std::string path= "../input_data/essential_matrix.txt";
    std::ofstream fout(path);
    if(!fout){
        std::cout << "file dosen't open" << '\n';
    }

    // для каждого вектора из ядра составляем матрицу Е
    // и вычисляем R и t_x из разожения SVD в calc_UV_S__R_t_x
    fout << ker.cols() << '\n'; // number of vectors kernel

    for (size_t i = 0; i < (size_t)ker.cols(); i++) {
        std::cout << "*******-" << i << "-*****************" << '\n';
        calculate_Ess_matrix(ker.col(i));
        // раскладываем полученную EssentMtrx с помощью SVD на UVS  и находим R и t_x
        calc_UV_S__R_t_x();

        std::cout << "The solution is:\n" << EssentMtrx << std::endl;
        // fout << EssentMtrx << "\n\n";
    }
    // fout.close();

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
