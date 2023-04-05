#include "blas/blas_matrix.h"
#include "openCL/cl_matrix.h"
#include "../utility/time.h"

typedef float type;

const size_t M = 300;
const size_t K = 500;
const size_t N = 400;


int main() {
    int l = 100;

    std::vector<type> a(M * K);
    for (int k = 0; k < M * K; ++k) {
        a[k] = -15 + k;
    }
    std::vector<type> b(N * K);
    for (int k = 0; k < N * K; ++k) {
        b[k] = -15 + k;
    }

    CLMatrix<type> cl_a(M, K, a.data());
    CLMatrix<type> cl_b(K, N, b.data());
    CLMatrix<type> cl_c(M, N);
    
    std::cout << s21::Time::Test([&] {
        MulCl(cl_a, cl_b, cl_c);
    }, l) << " ms CL\n";
    // cl_c.Print();

    std::cout << s21::Time::Test([&] {
        MulClBlas(cl_a, cl_b, cl_c);
    }, l) << " ms CLBlas\n";

    // cl_c.Print();
    // auto cl_res = cl_c.ToVector();



    BLASMatrix<type> blas_a(M, K, a);
    BLASMatrix<type> blas_b(K, N, b);
    BLASMatrix<type> blas_c(M, N);

    // std::cout << s21::Time::Test([&] {
    //     MulS(blas_a, blas_b, blas_c);
    // }, l) << " ms S\n";
    
    // blas_c.Print();
    // auto s_res = blas_c.ToVector();
    
    // std::cout << s21::Time::Test([&] {
    //     MulOmp(blas_a, blas_b, blas_c);
    // }, l) << " ms OMP\n";
    // blas_c.Print();
    // auto omp_res = blas_c.ToVector();

    std::cout << s21::Time::Test([&] {
        MulMkl(blas_a, blas_b, blas_c);
    }, l) << " ms MKL\n";
    
    // auto mkl_res = blas_c.ToVector();
    // blas_c.Print();

    // std::cout << (cl_res == s_res) << " " << (s_res == mkl_res) << " " << (cl_res == mkl_res) << "\n";
    // for (int k = 0; k < M * N; ++k) {
    //     std::cout << cl_res[k] << "," << s_res[k] << "," << mkl_res[k] << "\n";
    // }

    
    return 0;
}


// g++ main.cc -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -I/opt/intel/oneapi/mkl/2023.1.0/include/ -L/opt/intel/oneapi/mkl/2023.0.0/lib/intel64/ -lOpenCL

