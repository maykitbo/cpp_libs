
#include "matrix.h"
#include "gpu_matrix.h"
#include "../utility/time.h"

#include <iostream>

// export LD_LIBRARY_PATH=/opt/intel/oneapi/mkl/2023.0.0/lib/intel64/:$LD_LIBRARY_PATH


//export LD_LIBRARY_PATH=/opt/intel/oneapi/mkl/2023.1.0/include/:$LD_LIBRARY_PATH

// g++ gpu_matrix.cc -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl

using namespace s21;


int main() {

  // Free the memory.
//   for (int i = 0; i < m; i++) {
//     delete[] A[i];
//     delete[] C[i];
//   }
//   for (int i = 0; i < k; i++) {
//     delete[] B[i];
//   }
//   delete[] A;
//   delete[] B;
//   delete[] C;

  return 0;

    // Matrix<double> M1(3, 3, 5);
    // Matrix<double> M2(3, 3, 6);

    // GPUMatrix<double> GM1(3, 3, 5);
    // GPUMatrix<double> GM2(3, 3, 6);

    // auto M3 = M1 * M2;
    // auto GM3 = GM1 * GM2;

    // // M1.Print();
    // // M2.Print();
    // // GM1.Print();
    // // GM2.Print();
    // M3.Print();
    // GM3.Print();

    // std::cout << (M3 == *static_cast<Matrix<double>*>(&GM3)) << "\n";

    // double alpha = 1.0, beta = 0.0;
    // const int m = 1000, n = 1000, k = 1000;
    // double *V_1 = new double[m*k];
    // double *V_2 = new double[k*n];

    // Matrix<double> M_2(k, n);
    // Matrix<double> M_1(m, k, [&] (double &cell, int i, int j) {
    //     cell = 5.5;
    //     M_2(i, j) = 5.5;
    //     V_1[i * m + j] = 5.5;
    //     V_2[i * m + j] = 5.5;
    // });

    // Matrix<double> M_3(m, n);
    // double *V_3 = new double[m*n];

    // std::cout << Time::Test([&] {
    //     // for (int i = 0; i < 10; ++i) {
    //         cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, V_1, k, V_2, n, beta, V_3, n);
    //     // }
    // }) << "\n";



    // std::cout << Time::Test([&] {
    //     // for (int i = 0; i < 10; ++i) {
    //         M_3 = M_1 * M_2;
    //     // }
    // }) << "\n";


    return 0;
}

