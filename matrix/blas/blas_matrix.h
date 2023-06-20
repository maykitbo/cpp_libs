#pragma once

#include <vector>


#include <cblas.h>

#include <iostream>
#include <functional>
#include <fstream>

namespace BLAS {

template<class T>
class Arithmetic;

template<class T>
class Matrix {

    friend class Arithmetic<T>;
    size_t rows_, cols_;
    std::vector<T> matrix_;
    // Letters<T> letters_;

    public:
        Matrix() : matrix_(), rows_(0), cols_(0) {}
        Matrix(size_t row, size_t col) : rows_(row), cols_(col), matrix_(row * col) {}
        Matrix(size_t row, size_t col, T value) : rows_(row), cols_(col), matrix_(row * col, value) {}

        Matrix(size_t row, size_t col, const std::vector<T> &vec) : rows_(row), cols_(col), matrix_(vec) {}
        Matrix(const Matrix<T> &other) : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {}

        Matrix(size_t row, size_t col, std::function<T(void)> func) : Matrix(row, col) {
            std::generate(matrix_.begin(), matrix_.end(), func);
        }
        Matrix(size_t row, size_t col, std::function<void(T&)> func) : Matrix(row, col) {
            for (auto &i : matrix_) {
                func(i);
            }
        }
        Matrix(std::ifstream &file) {
            file >> rows_;
            file >> cols_;
            matrix_ = std::vector<T>(rows_ * cols_);
            for (auto &i : matrix_) {
                file >> i;
            }
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
        Matrix& operator=(const Matrix& other) {
            if (this != &other) {
                Matrix temp(other);
                std::swap(rows_, temp.rows_);
                std::swap(cols_, temp.cols_);
                std::swap(matrix_, temp.matrix_);
            }
            return *this;
        }
        Matrix& operator=(Matrix&& other) noexcept {
            std::swap(rows_, other.rows_);
            std::swap(cols_, other.cols_);
            std::swap(matrix_, other.matrix_);
            return *this;
        }
        void Update(const std::vector<T> &vec) {
            matrix_ = vec;
        }
        const size_t GetCols() const { return cols_; }
        const size_t GetRows() const { return rows_; }

        const std::vector<T> &ToVector() const { return matrix_; }
        std::vector<T> &ToVector() { return matrix_; }

        T &operator()(size_t row, size_t col) {
            return matrix_[cols_ * row + col];
        }
        const T &operator()(size_t row, size_t col) const {
            return matrix_[cols_ * row + col];
        }
        bool operator==(const Matrix &other) const {
            return matrix_ == other.matrix_ && cols_ == other.cols_;
        }
        friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
            os << m.rows_ << ' ' << m.cols_ << '\n';
            for (size_t i = 0; i < m.rows_; ++i) {
                for (size_t j = 0; j < m.cols_; ++j) {
                    os << m(i, j) << ' ';
                }
                os << '\n';
            }
            return os;
        }
        

        void Print() {
            for (int k = 0; k < rows_; ++k) {
                for (int g = 0; g < cols_; ++g) {
                    std::cout << matrix_[cols_ * k + g] << ' ';
                }
                std::cout << '\n';
            }
        }

        void PrintCR() {
            for (int k = 0; k < cols_; ++k) {
                for (int g = 0; g < rows_; ++g) {
                    std::cout << matrix_[cols_ * g + k] << ' ';
                }
                std::cout << '\n';
            }
        }
};

template<class T>
class Arithmetic {

    struct func;

    public:
        // C = alpha*A*B + beta*C
        static void Mul(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C, const T alpha = 1.0, const double beta = 0.0) {
            func::mul(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.rows_, B.cols_, A.cols_,
                alpha, A.matrix_.data(), A.cols_, B.matrix_.data(), B.cols_, beta, C.matrix_.data(), B.cols_);
        }
        static void MulAT(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C, const T alpha = 1.0, const double beta = 0.0) {
            func::mul(CblasRowMajor, CblasTrans, CblasNoTrans, A.cols_, B.cols_, A.rows_,
                alpha, A.matrix_.data(), A.cols_, B.matrix_.data(), B.cols_, beta, C.matrix_.data(), B.cols_);
        }
        static void MulBT(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C, const T alpha = 1.0, const double beta = 0.0) {
            func::mul(CblasRowMajor, CblasNoTrans, CblasTrans, A.rows_, B.rows_, A.cols_,
                alpha, A.matrix_.data(), A.cols_, B.matrix_.data(), B.cols_, beta, C.matrix_.data(), B.rows_);
        }
        static void Mul(const std::vector<T> &A, const Matrix<T> &B, Matrix<T> &C, const T alpha = 1.0, const double beta = 0.0) {
            func::mul(CblasRowMajor, CblasNoTrans, CblasNoTrans, 1, B.cols_, B.rows_,
                alpha, A.data(), B.rows_, B.matrix_.data(), B.cols_, beta, C.matrix_.data(), B.cols_);
        }

        static void Transpose(const Matrix<T> &A, Matrix<T> &B) {
            for (int k = 0; k < A.rows_; ++k) {
                for (int g = 0; g < A.cols_; ++g) {
                    B.matrix_[B.cols_ * g + k] = A.matrix_[A.cols_ * k + g];
                }
            }
        }

        // B = alpha*A + B
        static void Sum(const Matrix<T> &A, Matrix<T> &B, const T alpha = 1.0) {
            if (A.cols_ != B.cols_ || A.rows_ != B.rows_) {
                std::cout << "AAAAAA\n";
            }
            func::sum(A.cols_, alpha, A.matrix_.data(), 1, B.matrix_.data(), 1);
        }

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
struct Arithmetic<float>::func {
    constexpr static auto mul = cblas_sgemm;
    constexpr static auto sum = cblas_saxpy;
};

template<>
struct Arithmetic<double>::func {
    constexpr static auto mul = cblas_dgemm;
    constexpr static auto sum = cblas_daxpy;
};

} // namespace BLAS

