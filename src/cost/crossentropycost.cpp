#include "include/cost/crossentropycost.h"

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
double CrossEntropyCost::errorDelta(double output, double target)
{
    return output-target;
}
