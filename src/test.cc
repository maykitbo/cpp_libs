#include "matrix.h"
#include "gtest/gtest.h"

TEST(constructors, test_1) {
    Matrix<bool> A;
    Matrix<int> B{{1, 2, 3}, {1, 2, 3}};
    Matrix<std::pair<int, double>> C(6, 15, std::make_pair(3, 3.33));
}

TEST(constructors, test_2) {
    Matrix<float> A(7, 9, [&] (float &e, int i, int j) { e = i + j; });
    Matrix<float> B(A);
    ASSERT_EQ(A, B);
    Matrix<float> C(std::move(A));
    ASSERT_EQ(A, Matrix<float>());
    ASSERT_EQ(B, C);
}

TEST(operator, test_1) {
    Matrix<std::string> A(4, 5, "q");
    Matrix<std::string> B = A;
    ASSERT_EQ(A, B);
    Matrix<std::string> C = std::move(A);
    ASSERT_EQ(A, Matrix<std::string>());
    ASSERT_EQ(B, C);
}

TEST(operator, test_2) {
    Matrix<double> A(4, 6, 5.5);
    Matrix<double> B(A);
    Matrix<double> C(4, 6, 5.5);
    ASSERT_EQ(A == C, true);
    ASSERT_EQ(A != B, false);
    C.Set(3, 4, -1111);
    Matrix<double> D(4, 5, 5.5);
    ASSERT_EQ(A != D, true);
    ASSERT_EQ(A != C, true);
    ASSERT_EQ(A == C, false);
}

TEST(arithmetic, sum) {
    Matrix<int> A{{1, 2, 4, 6}, {-1, 0, 0, 0}};
    Matrix<int> B{{0, 3, 4, 11}, {0, -1, -2, 0}};
    Matrix<int> C{{1, 5, 8, 17}, {-1, -1, -2, 0}};
    ASSERT_EQ(A + B, C);
    A += B;
    ASSERT_EQ(A, C);
}

TEST(arithmetic, sub) {
    Matrix<int> A{{1, 2, 4, 6}, {-1, 0, 0, 0}};
    Matrix<int> B{{0, 3, 4, 11}, {0, -1, -2, 0}};
    Matrix<int> C{{1, -1, 0, -5}, {-1, 1, 2, 0}};
    ASSERT_EQ(A - B, C);
    A -= B;
    ASSERT_EQ(A, C);
}

TEST(arithmetic, mul) {
    Matrix<int> A{{1, 2, 4, 6}, {-1, 0, 0, 0}};
    Matrix<int> B{{0, 3}, {0, -1}, {7, 77}, {1, 4}};
    Matrix<int> C{{34, 333}, {0, -3}};
    ASSERT_EQ(A * B, C);
    A *= B;
    ASSERT_EQ(A, C);
    C = A * 5;
    A *= 5;
    Matrix<int> D{{170, 1665}, {0, -15}};
    ASSERT_EQ(A, C);
    ASSERT_EQ(A, D);
}

TEST(arithmetic, determinant) {
    Matrix<int> A{{7, -4}, {55, 1}};
    ASSERT_EQ(std::abs(A.Determinant() - 227.0) < 1e-3, true);
    Matrix<double> B{{1.999,2.56,-3.1}, {-0.44,8,0.01}, {2,-0.099,3.02}};
    ASSERT_EQ(std::abs(B.Determinant() - 101.216) < 1e-3, true);
}
