#pragma once

#include <iostream>
#include <functional>
#include <initializer_list>
#include <vector>
#include <cmath>
#include <memory>
#include <fstream>

namespace s21 {

template<class T>
class Matrix {
    private:
        T **matrix_;
        size_t rows_, cols_;
        void create(std::function<void(size_t, size_t)> func) {
            matrix_ = new T *[rows_];
            for (size_t k = 0; k < rows_; ++k) {
                matrix_[k] = new T [cols_];
                for (size_t g = 0; g < cols_; ++g) {
                    func(k, g);
                }
            }
        }

        void allocate() {
            matrix_ = new T *[rows_];
            for (size_t k = 0; k < rows_; ++k) {
                matrix_[k] = new T [cols_];
            }
        }

        void error(std::string name) const {
            throw std::invalid_argument("Matrix: " + name + " error");
        }

        void row_col_error(size_t row, size_t col, std::string name) const {
            if (row >= rows_ || row < 0 || col >= cols_ || col < 0) error(name);
        }

    public:
        void Loop(std::function<void(size_t, size_t)> func) {
            for (size_t k = 0; k < rows_; ++k) {
                for (size_t g = 0; g < cols_; ++g) {
                    func(k, g);
                }
            }
        }

        Matrix() noexcept : rows_(0), cols_(0) {}
        Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) { allocate(); }
        Matrix(size_t square) : rows_(square), cols_(square) { allocate(); }
        Matrix(size_t rows, size_t cols, const T &value) : rows_(rows), cols_(cols) {
            create([&] (size_t k, size_t g) { matrix_[k][g] = value; });
        }
        Matrix(size_t rows, size_t cols, std::function<const T(void)> value_func) : rows_(rows), cols_(cols) {
            create([&] (size_t k, size_t g) { matrix_[k][g] = value_func(); });
        }
        Matrix(size_t rows, size_t cols, std::function<void(T&)> value_func) : rows_(rows), cols_(cols) {
            create([&] (size_t k, size_t g) { value_func(matrix_[k][g]); });
        }
        Matrix(size_t rows, size_t cols, std::function<void(T&, size_t, size_t)> value_func) : rows_(rows), cols_(cols) {
            create([&] (size_t k, size_t g) { value_func(matrix_[k][g], k, g); });
        }
        Matrix(const Matrix &other) : rows_(other.rows_), cols_(other.cols_) {
            create([&] (size_t k, size_t g) { matrix_[k][g] = other(k, g); });
        }
        Matrix(std::initializer_list<std::initializer_list<T>> const &items) :
                rows_(items.size()), cols_(items.begin()->size()), matrix_(new T *[items.size()]) {
            size_t k = 0;
            for (auto const &row : items) {
                size_t g = 0;
                matrix_[k] = new T[row.size()];
                for (auto const &cell : row) {
                    matrix_[k][g++] = cell;
                }
                ++k;
            }
        }
        // Matrix(const std::vector<T> &vec) : 
        Matrix(Matrix &&other) noexcept : rows_(other.rows_), cols_(other.cols_), matrix_(std::move(other.matrix_)) {
            other.matrix_ = nullptr;
            other.rows_ = 0;
            other.cols_ = 0;
        }

        T *operator[](size_t row) {
            // return RowProxy(*this, row);
            return matrix_[row];
        }
        T &operator()(size_t row, size_t col) {
            row_col_error(row, col, "operator()");
            return matrix_[row][col];
        }
        const T &operator()(size_t row, size_t col) const {
            row_col_error(row, col, "const operator()");
            return matrix_[row][col];
        }

        void Resize(size_t new_rows, size_t new_cols) {
            if (new_rows <= 0 || new_cols <= 0) error("Resize");
            Matrix<T> new_matrix(new_rows, new_cols);
            size_t min_rows = std::min(rows_, new_rows);
            size_t min_cols = std::min(cols_, new_cols);
            for (size_t k = 0; k < min_rows; ++k) {
                for (size_t g = 0; g < min_cols; ++g) {
                    new_matrix(k, g) = matrix_[k][g];
                }
            }
            *this = std::move(new_matrix);
        }
        void ResizeRows(size_t new_rows) {
            Resize(new_rows, cols_);
        }
        void ResizeCols(size_t new_cols) {
            Resize(rows_, new_cols);
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

        void SwapRows(size_t row1, size_t row2) {
            if (row1 == row2) return;
            if (row1 >= rows_ || row1 < 0 || row2 >= rows_ || row2 < 0) error("SwapRows");
            for (size_t col = 0; col < cols_; ++col) {
                std::swap(matrix_[row1][col], matrix_[row2][col]);
            }
        }
        void SwapCols(size_t col1, size_t col2) {
            if (col1 >= cols_ || col1 < 0 || col2 >= cols_ || col2 < 0) error("SwapCols");
            if (col1 == col2) return;
            for (size_t row = 0; row < rows_; ++row) {
                std::swap(matrix_[row][col1], matrix_[row][col2]);
            }
        }

        void ForEach(std::function<void(const T&)> func) const {
            // Loop([&] (size_t k, size_t g) { func(matrix_[k * cols_ + g]); });
            for (size_t k = 0; k < rows_; ++k) {
                for (size_t g = 0; g < cols_; ++g) {
                    func(matrix_[k][g]);
                }
            }
        }
        void Fill(std::function<void(T&)> func) {
            Loop([&] (size_t k, size_t g) { func(matrix_[k][g]); });
        }

        const T &Get(size_t row, size_t col) const {
            row_col_error(row, col, "Get");
            return matrix_[row][col];
        }
        void Set(size_t row, size_t col, T value) {
            row_col_error(row, col, "Set");
            matrix_[row][col] = value;
        }
        size_t GetRows() const {
            return rows_;
        }
        size_t GetCols() const {
            return cols_;
        }

        template<class Mul>
        Matrix operator*(const Mul num) const {
            return Matrix(rows_, cols_, [&] (T &cell, size_t k, size_t g) { cell = matrix_[k][g] * num; });
        }
        template<class Mul>
        void operator*=(const Mul num) {
            Loop([&] (size_t k, size_t g) { matrix_[k][g] *= num; });
        }
        Matrix operator*(const Matrix &other) const {
            if (cols_ != other.rows_) error("Mul");
            return Matrix(rows_, other.cols_, [&] (T &cell, size_t k, size_t g) {
                cell = 0;
                for (size_t i = 0; i < cols_; ++i) {
                    cell += matrix_[k][i] * other(i, g);
                }
            });
        }
        std::vector<T> operator*(const std::vector<T>& other) {
            if (cols_ != other.size()) error("Mul");
            std::vector<T> result(rows_);
            for (size_t i = 0; i < rows_; ++i) {
                result[i] = 0;
                for (size_t j = 0; j < cols_; ++j) {
                    result[i] += matrix_[i][j] * other[j];
                }
            }
            return result;
        }
        void operator*=(const std::vector<T>& other) {
            if (cols_ != other.size()) error("Mul");
            *this = std::move(Matrix(rows_, 1, [&] (T &cell, size_t k, size_t g) {
                cell = 0;
                for (size_t i = 0; i < cols_; ++i) {
                    cell += matrix_[k][i] * other[i];
                }
            }));
        }
        template<class Vec>
        friend std::vector<T> operator*(const std::vector<Vec>& v, const Matrix& m) {
            if (v.size() != m.rows_) m.error("Mul");
            std::vector<T> result(m.cols_);
            for (size_t j = 0; j < m.cols_; ++j) {
                T sum = 0;
                for (size_t i = 0; i < m.rows_; ++i) {
                    sum += v[i] * m.matrix_[i][j];
                }
                result[j] = sum;
            }
            return result;
        }
        // template<class Vec>
        // friend void operator*=(std::vector<Vec>& v, const Matrix& m) {
        //     if (v.size() != m.rows_) m.error("Mul");
        //     std::vector<T> result(m.cols_);
        //     for (size_t j = 0; j < m.cols_; ++j) {
        //         T sum = 0;
        //         for (size_t i = 0; i < m.rows_; ++i) {
        //             sum += v[i] * m.matrix_[i][j];
        //         }
        //         result[j] = sum;
        //     }
        //     v = result;
        // }
        void operator*=(Matrix &other) {
            *this = std::move(*this * other);
        }
        Matrix operator+(Matrix &other) const {
            if (rows_ != other.rows_ || cols_ != other.cols_) error("Sum");
            return Matrix(rows_, cols_, [&] (T &cell, size_t k, size_t g) {
                cell = matrix_[k][g] + other(k, g);
            });
        }
        void operator+=(Matrix &other) {
            if (rows_ != other.rows_ || cols_ != other.cols_) error("Sum");
            Loop([&] (size_t k, size_t g) { matrix_[k][g] += other(k, g); });
        }
        Matrix operator-(Matrix &other) const {
            if (rows_ != other.rows_ || cols_ != other.cols_) error("Sub");
            return Matrix(rows_, cols_, [&] (T &cell, size_t k, size_t g) {
                cell = matrix_[k][g] - other(k, g);
            });
        }
        void operator-=(Matrix &other) {
            if (rows_ != other.rows_ || cols_ != other.cols_) error("Sub");
            Loop([&] (size_t k, size_t g) { matrix_[k][g] -= other(k, g); });
        }

        Matrix Transpose() const {
            return Matrix(cols_, rows_, [&] (T &cell, size_t k, size_t g) {
                cell = matrix_[g][k];
            });
        }
        void TransposeThis() {
            Matrix temp = Transpose();
            *this = std::move(temp);
        }

        bool operator==(const Matrix &other) const {
            if (cols_ != other.cols_ || rows_ != other.rows_) return false;
            for (size_t k = 0; k < rows_; ++k) {
                for (size_t g = 0; g < cols_; ++g) {
                    if (other(k, g) != matrix_[k][g]) {
                        return false;
                    }
                }
            }
            return true;
        }
        bool operator!=(Matrix &other) {
            return !operator==(other);
        }

        Matrix Minor(size_t row, size_t col) {
            row_col_error(row, col, "Minor");
            Matrix res(rows_ - 1, cols_ - 1);
            size_t i = 0, j;
            for (size_t k = 0; k < rows_; k++) {
                j = 0;
                if (k == row) continue;
                for (size_t g = 0; g < cols_; g++) {
                    if (g != col) {
                        res.matrix_[i][j] = matrix_[k][g];
                        j++;
                    }
                }
                i++;
            }
            return res;
        }
        // T Determinant() {
        //     if (rows_ != cols_) error("Determinant");
        //     std::vector<std::vector<T>> lu(rows_, std::vector<T>(cols_));
        //     std::vector<size_t> pivot(rows_);
        //     Loop([&] (size_t i, size_t j) { lu[i][j] = matrix_[i][j]; });
        //     size_t sign = 1;
        //     for (size_t i = 0; i < rows_; i++) {
        //         pivot[i] = i;
        //     }
        //     for (size_t k = 0; k < rows_ - 1; k++) {
        //         T max_value = 0;
        //         size_t max_index = k;
        //         for (size_t i = k; i < rows_; i++) {
        //             T abs_value = std::abs(lu[i][k]);
        //             if (abs_value > max_value) {
        //                 max_value = abs_value;
        //                 max_index = i;
        //             }
        //         }
        //         if (max_index != k) {
        //             std::swap(lu[k], lu[max_index]);
        //             std::swap(pivot[k], pivot[max_index]);
        //             sign *= -1;
        //         }
        //         for (size_t i = k + 1; i < rows_; i++) {
        //             T factor = lu[i][k] / lu[k][k];
        //             for (size_t j = k + 1; j < cols_; j++) {
        //                 lu[i][j] -= factor * lu[k][j];
        //             }
        //             lu[i][k] = factor;
        //         }
        //     }
        //     T det = sign;
        //     for (size_t i = 0; i < rows_; i++) {
        //         det *= lu[i][i];
        //     }
        //     return det;
        // }

        void Print() {
            std::cout << *this;
        }
        friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
            for (size_t i = 0; i < m.rows_; ++i) {
                for (size_t j = 0; j < m.cols_; ++j) {
                    os << m(i, j) << " ";
                }
                os << "\n";
            }
            return os;
        }
        void ToFile(std::ofstream &file) {
            file << rows_ << " " << cols_ << "\n";
            Loop([&] (size_t k, size_t g) {
                file << (matrix_[k][g]) << (g == cols_ - 1 ? '\n' : ' ');
            });
        }
        void ToFile(const std::string &file_name) {
            std::ofstream file(file_name);
            ToFile(file);
            file.close();
        }
        // void FromFile(const std::ifstream &file) {
        //     file >> rows_;
        //     file >> cols_;
        //     Matrix<T> new_matrix(rows_, cols_);
        //     Fill([&] (T &cell) {
        //         file >> cell;
        //     });
        //     *this = std::move(new_matrix);
        // }
        // void FromFile(const std::string &file_name) {
        //     std::ifstream file(file_name);
        //     FromFile(file);
        //     file.close();
        // }
        // friend std::ofstream& operator<<(std::ofstream &file, const Matrix<T>& m) {
        //     m.ToFile(file);
        // }
        // friend std::ifstream& operator<<(const std::ifstream &file, Matrix<T>& m) {
        //     m.FromFile(file);
        // }
        // void Prsize_t() const {
        //     std::cout << *this;
        // }

        virtual ~Matrix() {}
};

} // namespace s21
