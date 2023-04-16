// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

using std::ostream;
using std::ifstream;

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

class Matrix
{
public:
    Matrix(int rows, int cols);
    Matrix();
    Matrix(Matrix const &m);
    ~Matrix();

    int get_rows() const;
    int get_cols() const;
    Matrix& transpose();
    Matrix& vectorize();
    void plain_print();
    Matrix dot(const Matrix& m);
    double norm();


    Matrix operator+(const Matrix& b);
    Matrix operator+(const Matrix& b) const;

    Matrix& operator=(const Matrix& other);
    Matrix operator*(const Matrix& b);
    Matrix operator*(const Matrix& b) const;
    Matrix operator*(float c) const;
    friend Matrix operator*(float c, Matrix& mat);
    Matrix& operator+=(const Matrix& b);
    float& operator()(int i, int j);
    float& operator[](int i);

    float operator()(int i, int j) const;
    float operator[](int i) const;

    friend ostream& operator<<(ostream& os,Matrix& mat);
    friend ifstream& operator>>(ifstream &is, const Matrix& mat);

private:
    bool is_same_dims(const Matrix& b);
    float * _mat;
    int _full_size;
    matrix_dims * _dims;
};
#endif //MATRIX_H