#include "../include/quadraticcost.h"

#include <cmath>

QuadraticCost::QuadraticCost()
{

}

/**
 * @brief QuadraticCost::calculateCost
 * @param output
 * @param target
 * @return
 */
double QuadraticCost::calculateCost(double output, double target)
{
    return std::pow((output-target),2);
}

/**
 * @brief QuadraticCost::errorDelta
 * @param value
 * @param output
 * @param target
 * @return
 */
double QuadraticCost::errorDelta(double z, double output, double target)
{
    return (output-target) * sigmoidPrime(z);
}

double QuadraticCost::sigmoid(double z)
{
    return 1.0/(1.0 - std::exp(-z));
}

double QuadraticCost::sigmoidPrime(double z)
{
    return sigmoid(z) * (1.0 - sigmoid(z));
}
