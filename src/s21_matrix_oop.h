#ifndef SRC_S21_MATRIX_OOP_H_
#define SRC_S21_MATRIX_OOP_H_

#include <cmath>
#include <stdexcept>
#include <vector>

class S21Matrix {
 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);  // noexcept;
  ~S21Matrix();

  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  int getRows() const;        // accessor
  int getCols() const;        // accessor
  void setRows(int newRows);  // mutator
  void setCols(int newCols);  // mutator

  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other);  // noexcept;
  bool operator==(const S21Matrix& other);
  bool operator==(const S21Matrix& other) const;
  double& operator()(int rows, int cols);
  double operator()(int rows, int cols) const;
  void operator+=(const S21Matrix& other);
  S21Matrix operator+(const S21Matrix& other);
  void operator-=(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  void operator*=(const double num);
  S21Matrix operator*(const double num);
  void operator*=(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);

 private:
  int rows_, cols_;
  double** matrix_ = nullptr;

  void newMatrix(double*** matrix, int rows, int cols);
  void deleteMatrix(double*** matrix, int rows);
  S21Matrix createMinor(int row, int col) const;
};

#endif  // SRC_S21_MATRIX_OOP_H_
