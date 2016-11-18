#ifndef QUADRATICCOST_H
#define QUADRATICCOST_H

#include "cost.h"

class QuadraticCost : Cost
{
public:
    QuadraticCost();

    double calculateCost(double a, double y);
    double errorDelta(double target, double a, double y);
private:
    double sigmoid(double z);
    double sigmoidPrime(double z);

};

#endif // QUADRATICCOST_H
