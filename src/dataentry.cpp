#include "../include/dataentry.h"

DataEntry::DataEntry(int size = 10, Exercise ex = Exercise::UNKNOWN):
    _size(size), _excercise(ex)
{
    initializeEntries();
    initializeTypes();
}


DataEntry::DataEntry(std::vector<double> &e, Exercise ex = Exercise::UNKNOWN):
    _entries(e), _excercise(ex), _size(e.size())
{
    initializeTypes();
}

DataEntry::~DataEntry()
{

    _entries.clear();
    _types.clear();
}

std::vector<double>& DataEntry::getEntries()
{
    return _entries;
}

double& DataEntry::getEntry(const int index)
{
    return _entries[index];
}

void DataEntry::modifyEntry(double entry, DataType type, const int index)
{
    _entries[index] = entry;
    _types[index] = type;
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

