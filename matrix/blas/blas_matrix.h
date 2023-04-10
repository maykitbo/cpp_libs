#pragma once

#include <vector>


#include <cblas.h>

#include <iostream>
#include <functional>

namespace BLAS {

class Arithmetic;

template<class T>
class Matrix {

    friend class Arithmetic;
    size_t rows_, cols_;
    std::vector<T> matrix_;

    public:
        Matrix(size_t row, size_t col) : rows_(row), cols_(col), matrix_(row * col) {}

        Matrix(size_t row, size_t col, const std::vector<T> &vec) : rows_(row), cols_(col), matrix_(vec) {}

        Matrix(size_t row, size_t col, std::function<T(void)> func) : Matrix(row, col) {
            std::generate(matrix_.begin(), matrix_.end(), func);
        }

        Matrix(std::initializer_list<std::initializer_list<T>> const &items) :
               Matrix(items.size(), items.begin()->size()) {
            size_t k = 0;
            for (auto const &row : items) {
                for (auto const &cell : row) {
                    matrix_[k++] = cell;
                }
            }
        }
        void Update(const std::vector<T> &vec) {
            matrix_ = vec;
        }
        const size_t GetCols() const { return cols_; }
        const size_t GetRows() const { return rows_; }

        const std::vector<T> &ToVector() const { return matrix_; }
        std::vector<T> &ToVector() { return matrix_; }

        

        void Print() {
            for (int k = 0; k < rows_; ++k) {
                for (int g = 0; g < cols_; ++g) {
                    std::cout << matrix_[cols_ * k + g] << ' ';
                }
                std::cout << '\n';
            }
        }
};


class Arithmetic {

    template<class T>
    struct mul;

    public:
        template<class T>
        static void Mul(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C, const T alpha = 1.0, const double beta = 0.0) {
            mul<T>::func(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.rows_, B.cols_, A.cols_,
                alpha, A.matrix_.data(), A.cols_, B.matrix_.data(), B.cols_, beta, C.matrix_.data(), B.cols_);
        }
        template<class T>
        static void Mul(const std::vector<T> &A, const Matrix<T> &B, Matrix<T> &C, const T alpha = 1.0, const double beta = 0.0) {
            mul<T>::func(CblasRowMajor, CblasNoTrans, CblasNoTrans, 1, B.cols_, B.rows_,
                alpha, A.data(), B.rows_, B.matrix_.data(), B.cols_, beta, C.matrix_.data(), B.cols_);
        }

        template<class T>
        static void MulOmp(Matrix<T> &A, Matrix<T> &B, Matrix<T> &C) {
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
};

template<>
struct Arithmetic::mul<float> {
    constexpr static auto func = cblas_sgemm;
};

template<>
struct Arithmetic::mul<double> {
    constexpr static auto func = cblas_dgemm;
};

} // namespace BLAS

