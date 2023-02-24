#include "s21_matrix_oop.h"

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Sum error");
  }
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_; g++) {
      matrix_[k][g] += other(k, g);
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Sub error");
  }
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_; g++) {
      matrix_[k][g] -= other(k, g);
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_; g++) {
      matrix_[k][g] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument("Mul error");
  }
  double** res;
  newMatrix(&res, rows_, other.cols_);
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < other.cols_; g++) {
      res[k][g] = 0;
      for (int f = 0; f < cols_; f++) {
        res[k][g] += matrix_[k][f] * other(f, g);
      }
    }
  }
  deleteMatrix(&matrix_, rows_);
  cols_ = other.cols_;
  matrix_ = res;
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix res(cols_, rows_);
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_; g++) {
      res(g, k) = matrix_[k][g];
    }
  }
  return res;
}

S21Matrix S21Matrix::createMinor(int row, int col) const {
  S21Matrix res(rows_ - 1, cols_ - 1);
  int i = 0, j;
  for (int k = 0; k < rows_; k++) {
    j = 0;
    if (k == row) continue;
    for (int g = 0; g < cols_; g++) {
      if (g != col) {
        res.matrix_[i][j] = matrix_[k][g];
        j++;
      }
    }
    i++;
  }
  return res;
}

double S21Matrix::Determinant() const {
  double res = 0;
  if (rows_ != cols_) {
    throw std::invalid_argument("Determinant matrix is not square");
  }
  if (rows_ == 1) {
    res = matrix_[0][0];
  } else {
    for (int k = 0; k < cols_; k++) {
      res += createMinor(0, k).Determinant() * matrix_[0][k] * pow(-1, k);
    }
  }
  return res;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_ || rows_ == 1) {
    throw std::invalid_argument("Calc compliments error");
  }
  S21Matrix res(rows_, cols_);
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_; g++) {
      res.matrix_[k][g] =
          pow(-1, k + g) * this->createMinor(k, g).Determinant();
    }
  }
  return res;
}

S21Matrix S21Matrix::InverseMatrix() const {
  double det = this->Determinant();
  if (det == 0) {
    throw std::invalid_argument("Determinant = 0 in Invers matrix");
  }
  S21Matrix res(this->CalcComplements().Transpose());
  if (cols_ == 1) {
    res(0, 0) = 1.0 / det;
  } else {
    for (int k = 0; k < rows_; k++) {
      for (int g = 0; g < cols_; g++) {
        res(k, g) /= det;
      }
    }
  }
  return res;
}
