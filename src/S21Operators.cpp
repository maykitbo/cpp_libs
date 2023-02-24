#include "s21_matrix_oop.h"

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix res(*this);
  res.SumMatrix(other);
  return res;
}

void S21Matrix::operator+=(const S21Matrix& other) { this->SumMatrix(other); }

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix res(*this);
  res.SubMatrix(other);
  return res;
}

void S21Matrix::operator-=(const S21Matrix& other) { this->SubMatrix(other); }

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix res(*this);
  res.MulMatrix(other);
  return res;
}

void S21Matrix::operator*=(const S21Matrix& other) { this->MulMatrix(other); }

S21Matrix S21Matrix::operator*(const double value) {
  S21Matrix res(*this);
  res.MulNumber(value);
  return res;
}

void S21Matrix::operator*=(const double value) { this->MulNumber(value); }

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (matrix_ == other.matrix_) return *this;
  if (matrix_) deleteMatrix(&matrix_, rows_);
  rows_ = other.rows_;
  cols_ = other.cols_;
  newMatrix(&matrix_, rows_, cols_);
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_; g++) {
      matrix_[k][g] = other.matrix_[k][g];
    }
  }
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) {
  if (matrix_ == other.matrix_) return *this;
  if (matrix_) deleteMatrix(&matrix_, rows_);
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = other.matrix_;
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
  return *this;
}

double S21Matrix::operator()(int rows, int cols) const {
  if (rows >= 0 && rows < rows_ && cols >= 0 && cols < cols_)
    return matrix_[rows][cols];
  else
    throw std::invalid_argument("Incorrect matrix index error");
}

double& S21Matrix::operator()(int rows, int cols) {
  if (rows >= 0 && rows < rows_ && cols >= 0 && cols < cols_)
    return matrix_[rows][cols];
  else
    throw std::invalid_argument("Incorrect matrix index error");
}

bool S21Matrix::operator==(const S21Matrix& other) {
  return this->EqMatrix(other);
}

bool S21Matrix::operator==(const S21Matrix& other) const {
  return this->EqMatrix(other);
}
