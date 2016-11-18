#include "../include/quadraticcost.h"
#include "../include/network.h"

QuadraticCost::QuadraticCost()
{

}

double QuadraticCost::calculateCost(double output, double target)
{
    return 0.0;
}

/**
 * @brief QuadraticCost::errorDelta
 * @param value
 * @param output
 * @param target
 * @return
 */
double QuadraticCost::errorDelta(double value, double output, double target)
{
    return (output-target) * sigmoidPrime(value);
}

double QuadraticCost::sigmoid(double z)
{
    return 1.0/(1.0 - std::exp(-z));
}

double QuadraticCost::sigmoidPrime(double z)
{
    return sigmoid(z) * (1.0 - sigmoid(z));
}
