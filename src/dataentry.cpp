#include "../include/dataentry.h"

DataEntry::DataEntry(int size = 10):
    _size(size)
{
    initializeEntries();
    initializeTypes();
}


DataEntry::DataEntry(std::vector<double> &e, std::vector<DataType> &type):
    _entries(e), _size(e.size()), _types(type)
{

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

double DataEntry::getEntry(const int index) const
{
    return _entries[index];
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

void DataEntry::initializeEntries()
{
    for(int i = 0; i < _size; i++)
    {
        _entries.push_back(0);
    }
}

void DataEntry::initializeTypes()
{
    for(int i = 0; i < _size; i++)
    {
        _types.push_back(DataType::NO_DATA);
    }
}

