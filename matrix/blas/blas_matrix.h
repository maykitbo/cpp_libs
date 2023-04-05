#pragma once

#include <vector>
// #include <mkl.h>

#include <cblas.h>

#include <iostream>


template<class T>
class BLASMatrix {
    size_t rows_, cols_;
    std::vector<T> matrix_;
    public:
        BLASMatrix(size_t row, size_t col) : rows_(row), cols_(col), matrix_(row * col) {}

        BLASMatrix(size_t row, size_t col, const std::vector<T> &vec) : rows_(row), cols_(col), matrix_(vec) {} 

        friend void MulS(BLASMatrix &A, BLASMatrix &B, BLASMatrix &C) {
            if (A.cols_ != B.rows_ || A.rows_ != C.rows_ || B.cols_ != C.cols_) throw(std::invalid_argument("lox\n"));
            for (int k = 0; k < A.rows_; ++k) {
                for (int g = 0; g < B.cols_; ++g) {
                    T sum = 0;
                    for (int i = 0; i < A.cols_; ++i) {
                        sum += A.matrix_[A.cols_ * k + i] * B.matrix_[B.cols_ * i + g];
                    }
                    C.matrix_[C.cols_ * k + g] = sum;
                }
            }
        }
        friend void MulOmp(BLASMatrix &A, BLASMatrix &B, BLASMatrix &C) {
            #pragma omp parallel for collapse(2)
            for (int k = 0; k < A.rows_; ++k) {
                const int rows = B.cols_;
                const int cols = A.cols_;
                for (int g = 0; g < rows; ++g) {
                    T sum = 0;
                    #pragma omp simd reduction(+:sum) aligned(A.matrix_:64, B.matrix_:64)
                    for (int i = 0; i < cols; ++i) {
                        sum += A.matrix_[cols * k + i] * B.matrix_[rows * i + g];
                    }
                    C.matrix_[rows * k + g] = sum;
                }
            }
        }
        template<class Type>
        friend void MulMkl(BLASMatrix<Type> &A, BLASMatrix<Type> &B, BLASMatrix<Type> &C);


        std::vector<T> ToVector() {
            return matrix_;
        }

        void Print() {
            for (int k = 0; k < rows_; ++k) {
                for (int g = 0; g < cols_; ++g) {
                    std::cout << matrix_[cols_ * k + g] << ' ';
                }
                std::cout << '\n';
            }
        }
};


template<class Type>
void MulMkl(BLASMatrix<Type> &A, BLASMatrix<Type> &B, BLASMatrix<Type> &C) {
    throw(std::invalid_argument("MulMkl: template should be <float> or <double>\n"));
}

template<>
void MulMkl<double>(BLASMatrix<double> &A, BLASMatrix<double> &B, BLASMatrix<double> &C) {
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.rows_, B.cols_, A.cols_,
                1.0, A.matrix_.data(), A.cols_, B.matrix_.data(), B.cols_, 0.0, C.matrix_.data(), B.cols_);
}

template<>
void MulMkl<float>(BLASMatrix<float> &A, BLASMatrix<float> &B, BLASMatrix<float> &C) {
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.rows_, B.cols_, A.cols_,
                1.0, A.matrix_.data(), A.cols_, B.matrix_.data(), B.cols_, 0.0, C.matrix_.data(), B.cols_);
}

