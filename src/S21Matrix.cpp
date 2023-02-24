#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() : rows_(1), cols_(1) { newMatrix(&matrix_, 1, 1); }

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  newMatrix(&matrix_, rows_, cols_);
}

S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.rows_) {
  matrix_ = new double *[rows_]();
  for (int k = 0; k < rows_; k++) {
    matrix_[k] = new double[cols_]();
    for (int g = 0; g < cols_; g++) {
      matrix_[k][g] = other(k, g);
    }
  }
}

S21Matrix::S21Matrix(S21Matrix &&other)
    : rows_(other.rows_), cols_(other.rows_), matrix_(other.matrix_) {
  if (this != &other) {
    other.matrix_ = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
  }
}

S21Matrix::~S21Matrix() {
  if (matrix_) deleteMatrix(&matrix_, rows_);
  rows_ = 0;
  cols_ = 0;
}

void S21Matrix::newMatrix(double ***matrix, int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw std::invalid_argument("Create new matrix error");
  }
  *matrix = new double *[rows];
  for (int k = 0; k < rows; k++) {
    (*matrix)[k] = new double[cols]();
  }
}

void S21Matrix::deleteMatrix(double ***matrix, int rows) {
  for (int k = 0; k < rows; k++) {
    delete[](*matrix)[k];
  }
  delete[] * matrix;
}

void S21Matrix::setCols(int newCols) {
  double **buff;
  newMatrix(&buff, rows_, newCols);
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_ && g < newCols; g++) {
      buff[k][g] = matrix_[k][g];
    }
  }
  deleteMatrix(&matrix_, rows_);
  matrix_ = buff;
  cols_ = newCols;
}

void S21Matrix::setRows(int newRows) {
  double **buff;
  newMatrix(&buff, newRows, cols_);
  for (int k = 0; k < rows_ && k < newRows; k++) {
    for (int g = 0; g < cols_; g++) {
      buff[k][g] = matrix_[k][g];
    }
  }
  deleteMatrix(&matrix_, rows_);
  matrix_ = buff;
  rows_ = newRows;
}

int S21Matrix::getCols() const { return cols_; }

int S21Matrix::getRows() const { return rows_; }

bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;
  for (int k = 0; k < rows_; k++) {
    for (int g = 0; g < cols_; g++) {
      if (fabs(matrix_[k][g] - other(k, g)) > 1e-7) return false;
    }
  }
  return true;
}
