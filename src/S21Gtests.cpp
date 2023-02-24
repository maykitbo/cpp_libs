#include <cstdarg>

#include "gtest/gtest.h"
#include "s21_matrix_oop.h"

template <class T>
void fill(S21Matrix *A, ...) {  // only for tests
  va_list(args);
  va_start(args, A);
  for (int k = 0; k < A->getRows(); k++) {
    for (int g = 0; g < A->getCols(); g++) {
      (*A)(k, g) = (double)va_arg(args, T);
    }
  }
  va_end(args);
}

TEST(MatrixTest, Constructors) {
  S21Matrix A(2, 2);
  fill<int>(&A, 5, -4, 7, 123);
  S21Matrix B(2, 2);
  fill<int>(&B, 5, -4, 7, 123);
  EXPECT_EQ(A, B);
  S21Matrix C = B;
  A = std::move(C);
  EXPECT_EQ(A, B);
  S21Matrix E;
  E = A;
  EXPECT_EQ(A, E);
  S21Matrix D(std::move(A));
  EXPECT_EQ(D, B);
}

TEST(MatrixTest, EqMatrix) {
  S21Matrix A(2, 4);
  fill<int>(&A, 0, 9, 5, -13, 45, -45, 1, 4);
  S21Matrix B(2, 4);
  fill<int>(&B, 0, 9, 5, -13, 45, -45, 1, 4);
  EXPECT_EQ(A.EqMatrix(B), 1);
  fill<int>(&B, 4, 3, 2, 1, 1, 2, 3, 4);
  EXPECT_EQ(A.EqMatrix(B), 0);
  A.setCols(3);
  B.setCols(3);
  fill<int>(&A, 7, 12, 9, 13, 26, 15);
  fill<int>(&B, 7, 12, 9, 13, 26, 15);
  EXPECT_EQ(A.EqMatrix(B), 1);
}

TEST(MatrixTest, AcessorsMutators) {
  S21Matrix A(123, 10);
  EXPECT_EQ(A.getCols(), 10);
  EXPECT_EQ(A.getRows(), 123);
  A.setCols(3);
  A.setRows(4);
  EXPECT_EQ(A.getCols(), 3);
  EXPECT_EQ(A.getRows(), 4);
}

TEST(MatrixTest, SumMatrix) {
  S21Matrix A(2, 2);
  fill<int>(&A, 1, 2, 3, 4);
  S21Matrix B(2, 2);
  fill<int>(&B, 1, 2, 3, 4);
  A.SumMatrix(B);
  S21Matrix C(2, 2);
  fill<int>(&C, 2, 4, 6, 8);
  EXPECT_EQ(A, C);
}

TEST(MatrixTest, SubMatrix) {
  S21Matrix A(2, 2);
  fill<int>(&A, -1, 2, -3, 4);
  S21Matrix B(2, 2);
  fill<int>(&B, -1, 2, -3, 4);
  A.SubMatrix(B);
  S21Matrix C(2, 2);
  fill<int>(&C, 0, 0, 0, 0);
  EXPECT_EQ(A, C);
}

TEST(MatrixTest, SumMatrixError) {
  S21Matrix A(2, 3);
  S21Matrix B(4, 2);
  EXPECT_THROW(A.SumMatrix(B), std::invalid_argument);
}

TEST(MatrixTest, SubMatrixError) {
  S21Matrix A(2, 3);
  S21Matrix B(2, 2);
  EXPECT_THROW(A.SubMatrix(B), std::invalid_argument);
}

TEST(MatrixTest, MulNumber) {
  S21Matrix A(1, 4);
  fill<int>(&A, 1, 2, 3, 4);
  A.MulNumber(-3);
  S21Matrix C(1, 4);
  fill<int>(&C, -3, -6, -9, -12);
  EXPECT_EQ(A, C);
}

TEST(MatrixTest, MulMatrix) {
  S21Matrix A(2, 2);
  fill<int>(&A, 1, 2, 3, 4);
  S21Matrix B(2, 3);
  fill<int>(&B, -1, 2, -3, 4, 5, 6);
  A.MulMatrix(B);
  S21Matrix C(2, 3);
  fill<int>(&C, 7, 12, 9, 13, 26, 15);
  EXPECT_EQ(A, C);
}

TEST(MatrixTest, MulMatrixError) {
  S21Matrix A(2, 2);
  S21Matrix B(3, 4);
  EXPECT_THROW(A.MulMatrix(B), std::invalid_argument);
}

TEST(MatrixTest, Determinant) {
  S21Matrix A(3, 3);
  fill<int>(&A, 1, 3, -5, -7, 2, 4, -6, -8, 9);
  EXPECT_DOUBLE_EQ(A.Determinant(), -173.0);
}

TEST(MatrixTest, DeterminantError) {
  S21Matrix A(3, 2);
  EXPECT_THROW(A.Determinant(), std::invalid_argument);
}

TEST(MatrixTest, Transpose) {
  S21Matrix A(2, 3);
  fill<double>(&A, 1e123, 2.00005, 9.999999, -0.000006, 5e-32, 60.0909);
  S21Matrix B(3, 2);
  fill<double>(&B, 1e123, -0.000006, 2.00005, 5e-32, 9.999999, 60.0909);
  EXPECT_EQ(A.Transpose(), B);
}

TEST(MatrixTest, InverseMatrix) {
  S21Matrix A(3, 3);
  fill<int>(&A, 1, 2, 9, 8, -3, -4, -7, -6, 5);
  S21Matrix B = A.InverseMatrix();
  S21Matrix C(3, 3);
  fill<double>(&C, 0.05701754, 0.09356725, -0.02777778, 0.01754386, -0.0994152,
               -0.1111111, 0.10087719, 0.01169591, 0.02777778);
  EXPECT_EQ(B, C);
}

TEST(MatrixTest, InverseMatrixError) {
  S21Matrix A(3, 3);
  fill<int>(&A, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  EXPECT_THROW(A.InverseMatrix(), std::invalid_argument);
}

TEST(MatrixTest, Operators1) {
  S21Matrix A(2, 2);
  fill<int>(&A, 1, 2, 3, 4);
  S21Matrix B(2, 2);
  fill<int>(&B, 1, 2, 3, 4);
  S21Matrix C = A + B;
  S21Matrix D(2, 2);
  fill<int>(&D, 2, 4, 6, 8);
  EXPECT_EQ(D, C);
  C = A - B;
  fill<int>(&D, 0, 0, 0, 0);
  EXPECT_EQ(D, C);
  C = A * B;
  fill<int>(&D, 7, 10, 15, 22);
  EXPECT_EQ(D, C);
  C = A * 5;
  fill<int>(&D, 5, 10, 15, 20);
  EXPECT_EQ(C, D);
}

TEST(MatrixTest, Operators2) {
  S21Matrix A(3, 3);
  fill<int>(&A, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  S21Matrix B(3, 3);
  fill<int>(&B, 9, 8, 7, 6, 5, 4, 3, 2, 1);
  A *= B;
  S21Matrix C(3, 3);
  fill<int>(&C, 30, 24, 18, 84, 69, 54, 138, 114, 90);
  EXPECT_EQ(C, A);
  A -= B;
  fill<int>(&C, 21, 16, 11, 78, 64, 50, 135, 112, 89);
  EXPECT_EQ(C, A);
  A += A;
  fill<int>(&C, 42, 32, 22, 156, 128, 100, 270, 224, 178);
  EXPECT_EQ(C, A);
  A *= -0.5;
  fill<int>(&C, -21, -16, -11, -78, -64, -50, -135, -112, -89);
  EXPECT_EQ(C, A);
}

TEST(MatrixTest, Operators3) {
  S21Matrix A(2, 2);
  fill<int>(&A, 1, 2, 3, 4);
  EXPECT_EQ(A(1, 1), 4);
  EXPECT_THROW(A(3, 5), std::invalid_argument);
  S21Matrix B(2, 2);
  fill<int>(&B, 1, 2, 3, 4);
  EXPECT_EQ(A == B, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}