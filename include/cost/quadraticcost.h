#ifndef QUADRATICCOST_H
#define QUADRATICCOST_H

#include "cost.h"

class QuadraticCost : public Cost
{
public:
    QuadraticCost();

    double calculateCost(double output, double target);
    double errorDelta(double output, double target);
private:
};

#endif // QUADRATICCOST_H
