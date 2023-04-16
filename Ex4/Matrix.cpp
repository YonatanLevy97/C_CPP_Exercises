#include <iostream>
#include<fstream>

#include <cmath>
#include "Matrix.h"

#include "Activation.h"
#define MIN_PIXEL_VAL 0.1
#define INDEX(i, j, cols) (((i)*(cols)) + (j))

using std::ifstream;
using std::string;
using std::cin;
using std::ostream;
using std::cout;
using std::endl;



void set_matrix_arr(Matrix& mat, int r, int c, float def_val)
{
    for (int i = 0; i < (r*c); ++i)
    {
        mat[i] = def_val;
    }
}

bool Matrix::is_same_dims(const Matrix& b) {
    return (_dims->cols == b._dims->cols) && (_dims->rows == b._dims->rows);
}

void int_swap(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

Matrix::Matrix(int rows, int cols) :
        _mat(nullptr), _full_size(rows*cols), _dims(new matrix_dims)
{
    if (rows <= 0 || cols <= 0) { throw std::length_error(""); }
    _mat=new float[rows*cols];
    _dims->cols = cols, _dims->rows = rows;
    set_matrix_arr(*this, rows,cols,0);
}

Matrix::Matrix() : Matrix(1,1) {};

Matrix::Matrix(const Matrix &m):
        Matrix(m._dims->rows, m._dims->cols)
{
    for (int i = 0; i < (m._full_size); ++i)
    {
        _mat[i] = m._mat[i];
    }
}

Matrix::~Matrix()
{
    delete[] _mat;
    delete _dims;
}

int Matrix::get_rows() const
{
    return _dims->rows;
}

int Matrix::get_cols() const
{
    return _dims->cols;
}


Matrix& Matrix::transpose()
{
    int& row = _dims->rows;
    int& col = _dims->cols;
    float* transposed_mat = new float[row*col];
    for (int i = 0; i < col; ++i)
    {
        for (int j=0; j < row; ++j)
        {
            transposed_mat[INDEX(i,j,row)] = _mat[INDEX(j,i,col)];
        }
    }
    int_swap(row, col);
    _full_size = row * col;
    delete[] _mat;
    _mat = transposed_mat;
    return *this;
}

Matrix& Matrix::vectorize()
{
    _dims->rows *= _dims->cols;
    _dims->cols = 1;
    _full_size = get_cols() * get_rows();
    return *this;
}

void Matrix::plain_print()
{
    for (int i = 0; i < _dims->rows; ++i)
    {
        for (int j = 0; j < _dims->cols; j++)
        {
            cout << _mat[INDEX(i,j,_dims->cols)] << " ";
        }
        cout << "\n";
    }
}

Matrix Matrix::dot(const Matrix& m)
{
    if (!is_same_dims(m))
    {
        throw std::length_error("");
    }
    Matrix dotted = Matrix(m._dims->rows, m._dims->cols);
    for (int i = 0; i < (m._full_size); i++)
    {
        dotted._mat[i] = (_mat[i] * m._mat[i]);
    }
    return dotted;
}

double Matrix::norm() {
    double normal = 0.0;
    for(int i = 0; i < _full_size; i++)
    {
        float pow_a_i = (_mat[i] * _mat[i]);
        normal += pow_a_i;
    }
    return sqrt(normal);
}


Matrix& Matrix::operator=(const Matrix& other)
{
    if (this == &other) { return *this; }
    delete[] _mat, delete _dims;
    int rows = other._dims->rows, cols = other._dims->cols;
    _dims = new matrix_dims, _mat = new float[rows*cols];
    _dims->cols = cols, _dims->rows = rows;
    _full_size = rows*cols;
    for (int i = 0; i < rows*cols; ++i)
    {
        _mat[i] = other._mat[i];
    }
    return *this;
}



Matrix Matrix::operator*(const Matrix& other)
{
    int rows = get_rows(), cols = get_cols();
    int o_rows = other.get_rows(), o_cols = other.get_cols();
    if (cols != o_rows) { throw std::length_error("invalid matrix to mult"); }
    Matrix mult(*this);
    for (int i = 0 ; i < rows; i++)
    {
        for(int j = 0; j < o_cols; j++)
        {
            mult[INDEX(i,j,o_cols)] = 0;
            for(int k = 0; k < cols; k++)
            {
                float cord1 = _mat[INDEX(i,k,cols)];
                float cord2 = other._mat[INDEX(k,j,o_cols)];
                mult[INDEX(i,j,o_cols)] += cord1 * cord2;
            }
        }
    }
    mult._dims->cols = other.get_cols();
    mult._full_size = mult.get_cols() * mult.get_rows();
    return mult;
}



Matrix Matrix::operator*(const Matrix& other) const
{
    int rows = get_rows(), cols = get_cols();
    int o_rows = other.get_rows(), o_cols = other.get_cols();
    if (cols != o_rows) { throw std::length_error("invalid matrix to mult"); }
    Matrix mult(*this);
    for (int i = 0 ; i < rows; i++)
    {
        for(int j = 0; j < o_cols; j++)
        {
            mult[INDEX(i,j,o_cols)] = 0;
            for(int k = 0; k < cols; k++)
            {
                float cord1 = _mat[INDEX(i,k,cols)];
                float cord2 = other._mat[INDEX(k,j,o_cols)];
                mult[INDEX(i,j,o_cols)] += cord1 * cord2;
            }
        }
    }
    mult._dims->cols = other.get_cols();
    mult._full_size = mult.get_cols() * mult.get_rows();
    return mult;
}



Matrix Matrix::operator*(float c) const
{
    Matrix copy(*this);
    for (int i = 0; i < _full_size; i++)
    {
        copy[i] *= c;
    }
    return copy;
}

Matrix operator*(float c, Matrix &mat)
{
    return mat * c;
}

Matrix Matrix::operator+(const Matrix &b)
{
    Matrix a(*this);
    a+=b;
    return a;
}

Matrix Matrix::operator+(const Matrix &b) const
{
    Matrix a(*this);
    a+=b;
    return a;
}


Matrix& Matrix::operator+=(const Matrix& b)
{
    if (!is_same_dims(b)){ throw std::length_error(""); }
    for (int i = 0; i < _full_size; i++)
    {
        _mat[i] += b._mat[i];
    }
    return *this;
}

float& Matrix::operator[](int i)
{
    if (i >= _full_size || i < 0) { throw std::out_of_range(""); }
    return _mat[i];
}


float& Matrix::operator()(int i, int j)
{
    return (*this)[INDEX(i,j,get_cols())];
}

float Matrix::operator[](int i) const
{
    if (i >= _full_size || i < 0) { throw std::out_of_range(""); }
    return _mat[i];
}


float Matrix::operator()(int i, int j) const
{
    return (*this)[INDEX(i,j,get_cols())];
}


ostream& operator<<(ostream& os, Matrix &mat)
{
    for(int i = 0; i < mat.get_rows(); i++)
    {
        for(int j=0; j < mat.get_cols(); j++)
        {
            if(mat(i,j) > MIN_PIXEL_VAL)
            {
                os << "**";
            }
            else
            {
                os << "  ";
            }
        }
        os << "\n";
    }
    return os;
}

ifstream& operator>>(ifstream& is, const Matrix& mat)
{
    long int array_size_bytes = mat._full_size * sizeof (float);
    is.seekg (0, std::ios_base::end);
    long int file_size_bytes = is.tellg();
    if (file_size_bytes != array_size_bytes)
    {
        throw std::runtime_error("BINARY_FILE_SIZE_FAILURE");
    }
    is.seekg (0, std::ios_base::beg);
    if (!is.read ((char *) (mat._mat), array_size_bytes))
    {
        throw std::runtime_error("BINARY_READ_FAILURE");
    }
    if (!is) { throw std::runtime_error("BINARY_READ_FAILURE"); }
    return is;
}