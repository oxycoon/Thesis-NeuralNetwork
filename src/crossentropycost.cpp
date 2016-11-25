#include "../include/crossentropycost.h"

#include <cmath>

CrossEntropyCost::CrossEntropyCost()
{

}

double CrossEntropyCost::calculateCost(double output, double target)
{
    double value = -target * std::log(output) - (1-target) * std::log(1-output);

    if (value != value)
    {
        value = 0.0;
    }
    return value;
}

/**
 * @brief QuadraticCost::errorDelta
 * @param value
 * @param output
 * @param target
 * @return
 */
double CrossEntropyCost::errorDelta(double z, double output, double target)
{
    return output-target;
}

double CrossEntropyCost::sigmoid(double z)
{
    return 1.0/(1.0 - std::exp(-z));
}

double CrossEntropyCost::sigmoidPrime(double z)
{
    return sigmoid(z) * (1.0 - sigmoid(z));
}
