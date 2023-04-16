#include "Activation.h"
#include <cmath>

using std::exp;

Matrix activation::relu(const Matrix& mat)
{
    Matrix activated_mat(mat);
    for (int i=0; i < mat.get_cols() * mat.get_rows(); i++)
    {
        activated_mat[i] = ((mat[i] >= 0) ? mat[i] : 0);
    }
    return activated_mat;
}

Matrix activation::softmax(const Matrix& mat)
{
    Matrix activated_mat(mat);
    float sum_exp_mat = 0;
    for(int i = 0; i <  mat.get_cols() * mat.get_rows(); i++)
    {
        float exp_mat_i = exp(mat[i]);
        sum_exp_mat += exp_mat_i;
        activated_mat[i] = exp_mat_i;
    }
    return (1 / sum_exp_mat) * activated_mat;
}