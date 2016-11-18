#include "../include/datasegment.h"

#include <cmath>

DataSegment::DataSegment() :
    _totalAccelerometer(0.0), _totalGyroscope(0.0), _totalMagnetometer(0.0),
    _totalBarometer(0.0)
{

}

DataSegment::DataSegment(std::vector<DataEntry*> entries, std::vector<double> targets = {0}):
    _totalAccelerometer(0.0), _totalGyroscope(0.0), _totalMagnetometer(0.0),
    _totalBarometer(0.0), _segment(entries), _targets(targets)
{
    create();
}

void DataSegment::setTargets(const std::vector<double> targets)
{
    _targets = targets;
}

void DataSegment::setTarget(const int index, const double target)
{
    _targets[index] = target;
}

void DataSegment::addToTarget(const double target)
{
    _targets.push_back(target);
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

int DataSegment::getSegmentSize() const
{
    return _segment.size();
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

    //NOTE Fit for purpose, wouldn't work in a generalized set of data
    if(_segment[0]->getExercise() != Exercise::FALLING_FORWARD)
    {
        _targets.push_back(1.0);
        _targets.push_back(0.0);
    }
    else
    {
        _targets.push_back(0.0);
        _targets.push_back(1.0);
    }
}

double DataSegment::calculateTotal(std::vector<double> values)
{
    double result = 0.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    for(int i = 0; i < values.size()/3; i++)
    {
        x += values[i];
        y += values[i+1];
        z += values[i+2];
    }

    x = std::pow(x, 2);
    y = std::pow(y, 2);
    z = std::pow(z, 2);

    result = sqrt(x + y + z);

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

std::vector<double> DataSegment::getDataOfType(DataType type, bool getTotal) const
{
    std::vector<double> data;

    if(getTotal)
    {
        if(type == DataType::ACCELEROMETER)
            data.push_back(_totalAccelerometer);
        else if(type == DataType::GYRO)
            data.push_back(_totalGyroscope);
        else if(type == DataType::COMPASS)
            data.push_back(_totalMagnetometer);
        else if(type == DataType::BAROMETER)
            data.push_back(_totalBarometer);
    }

    for(int i = 0; i < _segment.size(); i++)
    {
        std::vector<double> tmp = _segment[i]->getEntriesOfDataType(type);
        for(int j = 0; j < tmp.size(); j++)
        {
            data.push_back(tmp[j]);
        }
    }
    return data;
}

std::vector<double> DataSegment::getTargets() const
{
    return _targets;
}

double DataSegment::getTarget(int index) const
{
    return _targets[index];
}
