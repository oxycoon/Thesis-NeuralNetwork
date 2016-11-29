#include "include/dataresults.h"

DataResults::DataResults()
{

}

void DataResults::addResult(ResultType type, double result)
{
    switch(type)
    {
    case ResultType::TMSE:
        _trainingMSE.push_back(result);
        break;
    case ResultType::TA:
        _trainingAccuracy.push_back(result);
        break;
    case ResultType::VMSE:
        _validationMSE.push_back(result);
        break;
    case ResultType::VA:
        _validationAccuracy.push_back(result);
        break;
    }
}

std::string DataResults::toString()
{
    std::string result;

    for(int i = 0; i < _trainingMSE.size(); i++)
    {
        result += std::to_string(_trainingMSE[i]) + ";";
        result += std::to_string(_trainingAccuracy[i]) + ";";
        result += std::to_string(_validationMSE[i]) + ";";
        result += std::to_string(_validationAccuracy[i]) + "\n";
    }
    return result;
}
