#include "matrix.h"
#include "gtest/gtest.h"
#include "../utility/random.h"

using namespace s21;

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

TEST(constructors, test_3) {
    Matrix<long double> B(100, 100, [&] { return Random::Uniform(-0.00001, 0.00001); });
    for (int k = 0; k < 100; ++k) {
        for (int g = 0; g < 100; ++g) {
            ASSERT_EQ(B(k, g) < 0.00001 && B(k, g) < 0.00001 > -0.00001, true);
        }
    }
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

TEST(operator, test_3) {
    Matrix<int> M{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {0, 0, 0}};
    ASSERT_EQ(M[2][2], 9);
    Matrix<long double> B(100, 100, [&] { return Random::Uniform(-0.00001, 0.00001); });
    Matrix<long double> A(B);
    A.Loop([&] (int k, int g) {
        ASSERT_EQ(A(k, g), B[k][g]);
    });
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

// TEST(arithmetic, determinant) {
//     Matrix<long double> A{{7, -4}, {55, 1}};
//     ASSERT_EQ(std::abs(A.Determinant() - 227.0) < 1e-3, true);
//     Matrix<double> B{{1.999,2.56,-3.1}, {-0.44,8,0.01}, {2,-0.099,3.02}};
//     ASSERT_EQ(std::abs(B.Determinant() - 101.216) < 1e-3, true);
// }

TEST(arithmetic, transpose) {
    Matrix<int64_t> T(10, 100);
    auto F = T.Transpose();
    T.TransposeThis();
    ASSERT_EQ(T, F);
}

TEST(arithmetic, mul_vector) {
    Matrix<int> M{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<int> V{-1, -2, -3};
    std::vector<int> mul{-14, -32, -50};
    auto res = M * V;
    ASSERT_EQ(res, mul);
    M *= V;
    Matrix<int> R{{-14}, {-32}, {-50}};
    ASSERT_EQ(M, R);
}

TEST(combined, fill__mul_num) {
    Matrix<long double> B(33, 95);
    Matrix<long double> A(B);
    B.Fill([&] (long double &cell) { cell = 12.12; });
    A.Fill([&] (long double &cell) { cell = 6.06; });
    A *= 2;
    ASSERT_EQ(A, B);
}

TEST(combined, string_matrix) {
    Matrix<std::string> S{{"qwe", "asd", "zxc"}, {"rty", "qw", "f"}};
    Matrix<std::string> K(S);
    S.Loop([&] (int i, int j) { ASSERT_EQ(S(i, j), K(i, j)); });
}

TEST(combined, matrix_matrix_1) {
    Matrix<Matrix<double>> MM({{{{1, 2, 3}, {1, 2, 3}, {4, 5, 6}}}, {{{7, 8, -9}, {-1, 0, -3}, {0, -5, -6}}}, {{{3, 3, 3}, {1, 1, 1}, {0, 0, 0}}}});
    MM *= 2;
}

TEST(combined, matrix_matrix_2) {
    Matrix<Matrix<double>> MM({{{{1, 2, 3}, {1, 2, 3}, {4, 5, 6}}}, {{{7, 8, -9}, {-1, 0, -3}, {0, -5, -6}}}, {{{3, 3, 3}, {1, 1, 1}}}});
    MM *= 2;
}

TEST(resize, test_1) {
    Matrix<int> A(10, 10, 5);
    Matrix<int> B(5, 5, 5);
    A.Resize(5, 5);
    ASSERT_EQ(A, B);
}

TEST(resize, test_2) {
    Matrix<int> A{{5, 5, 0, 0}, {5, 5, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    Matrix<int> B(2, 2, 5);
    B.Resize(4, 4);
    ASSERT_EQ(A, B);
}

TEST(swap, cols) {
    Matrix<int64_t> A{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {-1, -2, -3}};
    A.SwapCols(0, 2);
    ASSERT_EQ(A, Matrix<int64_t>({{3, 2, 1}, {6, 5, 4}, {9, 8, 7}, {-3, -2, -1}}));
}

TEST(swap, rows) {
    Matrix<char> A{{'a', 'b'}, {'c', 'd'}};
    A.SwapRows(0, 1);
    ASSERT_EQ(A, Matrix<char>({{'c', 'd'}, {'a', 'b'}}));
}

TEST(errors, row_col) {
    Matrix<int> F(2, 2);

    ASSERT_ANY_THROW(F.Resize(-2, 4));
    ASSERT_ANY_THROW(F.ResizeCols(-4));
    ASSERT_ANY_THROW(F.ResizeRows(0));

    ASSERT_ANY_THROW(F.SwapCols(-1, 1));
    ASSERT_ANY_THROW(F.SwapCols(0, 10));
    ASSERT_ANY_THROW(F.SwapRows(1, 2));
    ASSERT_ANY_THROW(F.SwapRows(-10, 1));

    ASSERT_ANY_THROW(F.Get(-1, 1));
    ASSERT_ANY_THROW(F.Set(1, 10, 5));

    ASSERT_ANY_THROW(F(-1, 1));
    ASSERT_ANY_THROW(F(1, 3));

    ASSERT_ANY_THROW(F.Minor(1, -5));
    ASSERT_ANY_THROW(F.Minor(2, 0));
}

TEST(errors, arithmetic) {
    Matrix<int> F(2, 2);
    Matrix<int> G(2, 3);
    Matrix<int> K(3, 2);

    ASSERT_ANY_THROW(F + G);
    ASSERT_ANY_THROW(G += K);
    ASSERT_ANY_THROW(K - F);
    ASSERT_ANY_THROW(F -= K);

    ASSERT_ANY_THROW(G * F);
    ASSERT_ANY_THROW(F *= K);

    // ASSERT_ANY_THROW(G.Determinant());
    // ASSERT_ANY_THROW(K.Determinant());
}

