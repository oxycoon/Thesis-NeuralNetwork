#ifndef CROSSENTROPYCOST_H
#define CROSSENTROPYCOST_H

#include "cost.h"

class CrossEntropyCost : public Cost
{
public:
    CrossEntropyCost();

    double calculateCost(double output, double target);
    double errorDelta(double z, double output, double target);
private:
    double sigmoid(double z);
    double sigmoidPrime(double z);

};

#endif // CROSSENTROPYCOST_H
