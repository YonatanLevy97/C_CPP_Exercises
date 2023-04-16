#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H


// Insert Activation class here...
namespace activation
{
    Matrix relu(const Matrix& mat);

    Matrix softmax(const Matrix& mat);

    typedef Matrix (*actv_func)(const Matrix& mat);
};

#endif //ACTIVATION_H