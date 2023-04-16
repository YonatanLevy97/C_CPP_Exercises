#include "Dense.h"

using namespace activation;


Dense::Dense(const Matrix &weights, const Matrix &bias,
             actv_func activ_function):
_weights(weights), _bias(bias), _activ_function(activ_function){};


Matrix Dense::get_weights() const { return _weights; }

Matrix Dense::get_bias() const {return _bias;}

actv_func Dense::get_activation() { return _activ_function; }

Matrix Dense::operator()(const Matrix& in) const
{
    return _activ_function((_weights * in) + _bias);
}




