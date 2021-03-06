#ifndef COST_H
#define COST_H

enum CostCalc{
    Quadratic,
    CrossEntropy
};

class Cost
{
public:
    Cost();

    virtual double calculateCost(double output, double target) = 0;
    virtual double errorDelta(double output, double target) = 0;
    CostCalc getCalculatorType();

protected:
    CostCalc _calculatorType;
};

#endif // COST_H
