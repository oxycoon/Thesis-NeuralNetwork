#include "../include/dataentrytotal.h"

#include <cmath>

DataEntryTotal::DataEntryTotal(std::vector<DataEntry*> entries):
    _totalAccelerometer(0.0), _totalGyroscope(0.0), _totalMagnetometer(0.0),
    _totalBarometer(0.0)
{
    create(entries);
}

double DataEntryTotal::getTotalAccelerometer()
{
    return _totalAccelerometer;
}

double DataEntryTotal::getTotalGyroscope()
{
    return _totalGyroscope;
}

double DataEntryTotal::getTotalMagnetometer()
{
    return _totalMagnetometer;
}

double DataEntryTotal::getTotalBarometer()
{
    return _totalBarometer;
}

void DataEntryTotal::create(std::vector<DataEntry*> entries)
{
    std::vector<double> tempAcce;
    std::vector<double> tempGyro;
    std::vector<double> tempMagn;
    std::vector<double> tempBaro;

    for(int i = 0; i < entries.size(); i++)
    {
        std::vector<double> a = entries[i]->getEntriesOfDataType(DataType::ACCELEROMETER);
        std::vector<double> b = entries[i]->getEntriesOfDataType(DataType::GYRO);
        std::vector<double> c = entries[i]->getEntriesOfDataType(DataType::COMPASS);
        std::vector<double> d = entries[i]->getEntriesOfDataType(DataType::BAROMETER);
        if(a.size() > 0)
        {
            for(int j = 0; j < a.size(); j++)
            {
                tempAcce.push_back(a[j]);
            }
        }
        if(b.size() > 0)
        {
            for(int j = 0; j < b.size(); j++)
            {
                tempGyro.push_back(b[j]);
            }
        }
        if(c.size() > 0)
        {
            for(int j = 0; j < c.size(); j++)
            {
                tempMagn.push_back(c[j]);
            }
        }
        if(d.size() > 0)
        {
            for(int j = 0; j < d.size(); j++)
            {
                tempBaro.push_back(d[j]);
            }
        }
    }
    if(tempAcce.size() > 0) _totalAccelerometer = calculateTotal(tempAcce);
    if(tempGyro.size() > 0) _totalGyroscope = calculateTotal(tempGyro);
    if(tempMagn.size() > 0) _totalMagnetometer = calculateTotal(tempMagn);
    if(tempBaro.size() > 0) _totalBarometer = calculateTotal(tempBaro);
}

double DataEntryTotal::calculateTotal(std::vector<double> values)
{
    double result = 0.0;

    for(int i = 0; i < values.size(); i++)
    {
        result += std::pow(values[i], 2);
    }

    result = result / values.size();
    result = sqrt(result);

    return result;
}
