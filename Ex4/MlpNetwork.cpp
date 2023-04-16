#include "MlpNetwork.h"
#include "Activation.h"

using namespace activation;
MlpNetwork::MlpNetwork(Matrix weights[4], Matrix biases[4]):
_weights(weights), _biases(biases){}

digit MlpNetwork::operator()(const Matrix &mat)
{
    Dense r1_d(_weights[0], _biases[0], relu);
    Matrix r1 = r1_d(mat);

    Dense r2_d(_weights[1], _biases[1], relu);
    Matrix r2 = r2_d(r1);

    Dense r3_d(_weights[2], _biases[2], relu);
    Matrix r3 = r3_d(r2);

    Dense r4_d(_weights[3], _biases[3], softmax);
    Matrix r4 = r4_d(r3);

    digit result {0,0};
    for (int i = 0; i < r4.get_cols() * r4.get_rows(); i++)
    {
        if(result.probability < r4[i])
        {
            result.value = i;
            result.probability = r4[i];
        }
    }
    return result;
}

//digit MlpNetwork::operator()(const Matrix& mat) const {
//    Matrix r1 = relu((_weights[0] * mat) + _biases[0]);
//    Matrix r2 = relu((_weights[1] * r1) + _biases[1]);
//    Matrix r3 = relu((_weights[2] * r2) + _biases[2]);
//    Matrix r4 = softmax((_weights[3] * r3) + _biases[3]);
//    digit result {0,0};
//    for (int i = 0; i < r4.get_cols() * r4.get_rows(); i++)
//    {
//        if(result.probability < r4[i])
//        {
//            result.value = i;
//            result.probability = r4[i];
//        }
//    }
//    return result;
//};


