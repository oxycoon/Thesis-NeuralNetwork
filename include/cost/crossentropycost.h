#ifndef CROSSENTROPYCOST_H
#define CROSSENTROPYCOST_H

#include "include/cost/cost.h"

class CrossEntropyCost : public Cost
{
public:
    CrossEntropyCost();

    double calculateCost(double output, double target);
    double errorDelta(double output, double target);
};

#endif // CROSSENTROPYCOST_H
