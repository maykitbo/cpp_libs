#include "blas/blas_matrix.h"
#include "openCL/cl_matrix.h"

#include "gtest/gtest.h"

typedef float type;

const size_t M = 400;
const size_t K = 300;
const size_t N = 200;

template<class T>
static void Compare(const CL::Matrix<T> &CM, const BLAS::Matrix<T> &BM) {
    ASSERT_EQ(CM.GetCols(), BM.GetCols());
    ASSERT_EQ(CM.GetRows(), BM.GetRows());
    ASSERT_EQ(CM.ToVector(), BM.ToVector());
}

TEST(compare, constructor) {
    CL::Matrix<float> CM1{{1, 2, 3, 4}, {5, 6, 7, 8}, {0, 0, 0, 0}};
    BLAS::Matrix<float> BM1{{1, 2, 3, 4}, {5, 6, 7, 8}, {0, 0, 0, 0}};
    Compare(CM1, BM1);
    CL::Matrix<long double> CM2(10, 10);
    BLAS::Matrix<long double> BM2(10, 10);
    Compare(CM2, BM2);
    CL::Matrix<long long int> CM3(2, 3, std::vector<long long int>{1, 2, 3, 4, 5, 6});
    BLAS::Matrix<long long int> BM3(2, 3, std::vector<long long int>{1, 2, 3, 4, 5, 6});
    Compare(CM3, BM3);
}

TEST(compare, mul) {
    CL::Matrix<float> CM(2, 1);
    CL::Arithmetic::Mul(CL::Matrix<float>{{1, 2, 3, 4}, {5, 6, 7, 8}}, CL::Matrix<float>{{11}, {22}, {33}, {44}}, CM);
    BLAS::Matrix<float> BM(2, 1);
    BLAS::Arithmetic::Mul(BLAS::Matrix<float>{{1, 2, 3, 4}, {5, 6, 7, 8}}, BLAS::Matrix<float>{{11}, {22}, {33}, {44}}, BM);
    Compare(CM, BM);
}

// int main() {
//     int l = 100;

//     std::vector<type> a(M * K);
//     for (int k = 0; k < M * K; ++k) {
//         a[k] = s21::Random::Normal<type>(-1, 1000);
//     }
//     std::vector<type> b(N * K);
//     for (int k = 0; k < N * K; ++k) {
//         b[k] = s21::Random::Normal<type>(1, 3000);
//     }

//     CL::Matrix<type> cl_a(M, K, a.data());
//     CL::Matrix<type> cl_b(K, N, b.data());
//     CL::Matrix<type> cl_c(M, N);
    
//     std::cout << s21::Time::Test([&] {
//         CL::Arithmetic::Mul(cl_a, cl_b, cl_c);
//     }, l) << " ms CLblas\n";

//     std::cout << s21::Time::Test([&] {
//         CL::Arithmetic::MulCL(cl_a, cl_b, cl_c);
//     }, l) << " ms CL\n";



//     BLAS::Matrix<type> blas_a(M, K, a);
//     BLAS::Matrix<type> blas_b(K, N, b);
//     BLAS::Matrix<type> blas_c(M, N);



//     std::cout << s21::Time::Test([&] {
//         BLAS::Arithmetic::Mul(blas_a, blas_b, blas_c);
//     }, l) << " ms BLAS\n";
    
//     // auto mkl_res = blas_c.ToVector();
//     // blas_c.Print();

//     // std::cout << (cl_res == s_res) << " " << (s_res == mkl_res) << " " << (cl_res == mkl_res) << "\n";
//     // for (int k = 0; k < M * N; ++k) {
//     //     std::cout << cl_res[k] << "," << s_res[k] << "," << mkl_res[k] << "\n";
//     // }

    
//     return 0;
// }


