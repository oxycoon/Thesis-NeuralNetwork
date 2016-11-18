#ifndef COST_H
#define COST_H


class Cost
{
public:
    Cost();

    virtual double calculateCost(double a, double y) = 0;
    virtual double errorDelta(double target, double a, double y) = 0;
};

#endif // COST_H
