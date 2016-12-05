#include "include/dataentry.h"

#include <sstream>

DataEntry::DataEntry(int size = 10):
    _size(size)
{
    initializeEntries();
    initializeTypes();
}


DataEntry::DataEntry(std::vector<double> &e, std::vector<DataType> &type, Exercise ex):
    _entries(e), _size(e.size()), _types(type), _exercise(ex)
{
    //Failsafe if there are more data nodes than we know the data types of
    if(_size > type.size())
    {
        for(int i = type.size(); i < _size; i++)
        {
            _types.push_back(DataType::UK);
        }
    }
}

DataEntry::~DataEntry()
{
    _entries.clear();
    _types.clear();
}

std::vector<double> DataEntry::getEntries() const
{
    return _entries;
}

std::vector<double> DataEntry::getEntriesOfDataType(DataType type) const
{
    if(type == DataType::UK)
    {
        return _entries;
    }

    std::vector<double> returnVector;

    for(int i = 0; i < _types.size(); i++)
    {
        if(type == DataType::ACCELEROMETER)
        {
            if(_types[i] == DataType::ACCEL_X ||
                _types[i] == DataType::ACCEL_Y ||
                _types[i] == DataType::ACCEL_Z)
                    returnVector.push_back(_entries[i]);
        }
        else if(type == DataType::COMPASS)
        {
            if(_types[i] == DataType::COM_X ||
                _types[i] == DataType::COM_Y ||
                _types[i] == DataType::COM_Z)
                    returnVector.push_back(_entries[i]);
        }
        else if(type == DataType::GYRO)
        {
            if(_types[i] == DataType::GYR_X ||
                _types[i] == DataType::GYR_Y ||
                _types[i] == DataType::GYR_Z)
                    returnVector.push_back(_entries[i]);
        }
        else if(type == DataType::BAROMETER)
        {
            if(_types[i] == DataType::BAR)
                 returnVector.push_back(_entries[i]);
        }
    }
    return returnVector;
}

double DataEntry::getEntry(const int index) const
{
    return _entries[index];
}

DataType DataEntry::getDataType(const int index) const
{
    return _types[index];
}

Exercise DataEntry::getExercise() const
{
    return _exercise;
}

int DataEntry::getSize() const
{
    return _size;
}

void DataEntry::modifyEntry(double entry, DataType type, const int index)
{
    _entries[index] = entry;
    _types[index] = type;
}

void DataEntry::setTimestamp(long time)
{
    _timestamp = time;
}

void DataEntry::setExercise(Exercise ex)
{
    _exercise = ex;
}

std::string DataEntry::toString()
{
    std::string result = "";

    for(int i = 0; i < _entries.size(); i++)
    {
        result += std::to_string(_entries[i]);
        if(i != (_entries.size() - 1))
            result += ",";
    }

    return result;
}

void DataEntry::initializeEntries()
{
    for(int i = 0; i < _size; i++)
    {
        _entries.push_back(0.0);
    }
}

void DataEntry::initializeTypes()
{
    for(int i = 0; i < _size; i++)
    {
        _types.push_back(DataType::NODATA);
    }
}

