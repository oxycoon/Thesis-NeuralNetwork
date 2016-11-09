#ifndef DATARESULTS_H
#define DATARESULTS_H

#include <vector>
#include <string>

enum ResultType{
    TMSE,
    TA,
    VMSE,
    VA
};

class DataResults
{
public:
    DataResults();

    void addResult(ResultType type, double result);

    std::string toString();
private:

    std::vector<double> _trainingMSE;
    std::vector<double> _trainingAccuracy;
    std::vector<double> _validationMSE;
    std::vector<double> _validationAccuracy;
};

#endif // DATARESULTS_H
