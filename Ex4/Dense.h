#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"
#include "Activation.h"

class Dense
{
public:
    Dense(const Matrix& weights, const Matrix& bias,
                            activation::actv_func activ_function);
    Matrix get_weights() const;
    Matrix get_bias() const;
    activation::actv_func get_activation();
    Matrix operator()(const Matrix& in) const;
private:
     const Matrix _weights;
     const Matrix _bias;
    activation::actv_func _activ_function;
};
#endif //DENSE_H





