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
double QuadraticCost::errorDelta(double output, double target)
{
    return (target-output) * output * (1 - output);
}

