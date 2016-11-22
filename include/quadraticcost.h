#ifndef QUADRATICCOST_H
#define QUADRATICCOST_H

#include "cost.h"

class QuadraticCost : Cost
{
public:
    QuadraticCost();

    double calculateCost(double a, double y);
    double errorDelta(double value, double output, double target);
private:
    double sigmoid(double z);
    double sigmoidPrime(double z);

};

#endif // QUADRATICCOST_H
