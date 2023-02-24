CC=g++
SRC=S21Arithmetic.cpp S21Matrix.cpp S21Operators.cpp
OBJ=S21Arithmetic.o S21Matrix.o S21Operators.o
CFLAGS=-std=c++17
TESTFLAGS=-lgtest_main -lgtest -lpthread -lgcov

all: test gcov_report s21_matrix_oop.a

gcov_report: test
	gcovr -r. --html --html-details -o report/coverage_report.html

test: s21_matrix_oop.a
	$(CC) $(CFLAGS) -pthread S21Gtests.cpp s21_matrix_oop.a $(TESTFLAGS) -o test
	./test

s21_matrix_oop.a: s21_matrix.o
	rm -rf *.a
	ar rcs s21_matrix_oop.a $(OBJ)

s21_matrix.o:
	rm -rf *.o
	rm -rf *.gc*
	$(CC) $(CFLAGS) --coverage -c $(SRC)

clean:
	rm -rf *.o
	rm -rf *.gc*
	rm -rf *.a
	rm -rf test
	rm -rf a.out
	rm -rf report/*
