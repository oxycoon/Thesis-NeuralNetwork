#ifndef QUADRATICCOST_H
#define QUADRATICCOST_H

#include "cost.h"

class QuadraticCost : public Cost
{
public:
    QuadraticCost();

    double calculateCost(double output, double target);
    double errorDelta(double z, double output, double target);
private:
    double sigmoid(double z);
    double sigmoidPrime(double z);

};

#endif // QUADRATICCOST_H
