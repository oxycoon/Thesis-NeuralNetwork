#include "../include/datasegment.h"

#include <cmath>

DataSegment::DataSegment() :
    _totalAccelerometer(0.0), _totalGyroscope(0.0), _totalMagnetometer(0.0),
    _totalBarometer(0.0)
{

}

DataSegment::DataSegment(std::vector<DataEntry*> entries):
    _totalAccelerometer(0.0), _totalGyroscope(0.0), _totalMagnetometer(0.0),
    _totalBarometer(0.0), _segment(entries)
{
    create();
}

double DataSegment::getTotalAccelerometer() const
{
    return _totalAccelerometer;
}

double DataSegment::getTotalGyroscope() const
{
    return _totalGyroscope;
}

double DataSegment::getTotalMagnetometer() const
{
    return _totalMagnetometer;
}

double DataSegment::getTotalBarometer() const
{
    return _totalBarometer;
}

void DataSegment::addToSegment(DataEntry *entry)
{
    _segment.push_back(entry);
}

void DataSegment::create()
{
    std::vector<double> tempAcce;
    std::vector<double> tempGyro;
    std::vector<double> tempMagn;
    std::vector<double> tempBaro;

    for(int i = 0; i < _segment.size(); i++)
    {
        std::vector<double> a = _segment[i]->getEntriesOfDataType(DataType::ACCELEROMETER);
        std::vector<double> b = _segment[i]->getEntriesOfDataType(DataType::GYRO);
        std::vector<double> c = _segment[i]->getEntriesOfDataType(DataType::COMPASS);
        std::vector<double> d = _segment[i]->getEntriesOfDataType(DataType::BAROMETER);
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

double DataSegment::calculateTotal(std::vector<double> values)
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

std::vector<DataEntry *> DataSegment::getSegments() const
{
    return _segment;
}

DataEntry *DataSegment::getSegment(int index) const
{
    return _segment[index];
}

std::vector<double> DataSegment::getDataOfType(DataType type) const
{
    std::vector<double> data;

    for(int i = 0; i < _segment.size(); i++)
    {
        std::vector<double> tmp = _segment[i]->getEntriesOfDataType(type);
        for(int j = 0; j < tmp.size(); j++)
        {
            data.push_back(tmp[i]);
        }
    }
    return data;
}
