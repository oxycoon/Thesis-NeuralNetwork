#ifndef COST_H
#define COST_H


class Cost
{
public:
    Cost();

    virtual double calculateCost(double output, double target) = 0;
    virtual double errorDelta(double z, double output, double target) = 0;
};

#endif // COST_H
