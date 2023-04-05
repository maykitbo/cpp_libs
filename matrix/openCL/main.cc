#include "cl_matrix.h"

typedef float type;

const size_t M = 2;
const size_t K = 3;
const size_t N = 4;

const std::vector<type> a{1, 2, 3, 4, 5, 6};
const std::vector<type> b{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int main() {
    CLMatrix<type> cl_a(M, K, a.data());
    CLMatrix<type> cl_b(K, N, b.data());
    CLMatrix<type> cl_c(M, N);
    MulCl(cl_a, cl_b, cl_c);
    cl_c.Print();

    return 0;
}
// int main() {
//     const std::vector<float> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
//     const std::vector<float> b{1, 2, 3, 4, 5, 6, 7, 8};
//     CLMatrix<float> A(4, 4, a.data());
//     CLMatrix<float> B(4, 2, b.data());
//     CLMatrix<float> C(4, 2);
//     Mul(A, B, C);

//     A.Print();
//     B.Print();
//     C.Print();

//     return 0;
// }
