#include "../include/datacollection.h"

#include <iostream>

DataCollection::DataCollection(Exercise ex = Exercise::UNKNOWN):
    _exercise(ex)
{

}

DataCollection::~DataCollection()
{
    for(int i = 0; i < _collection.size(); i++)
    {
        delete _collection[i];
    }
    _collection.clear();
}

DataEntry *DataCollection::getEntry(const int index) const
{
    return _collection[index];
}

int DataCollection::getCollectionSize() const
{
    return _collection.size();
}

Exercise DataCollection::getExercise() const
{
    return _exercise;
}

void DataCollection::setExercise(const Exercise ex)
{
    _exercise = ex;
}

std::vector<double> DataCollection::getTarget() const
{
    std::vector<double> target = std::vector<double>(1);
    target[0] = _exercise;
    return target;
}

void DataCollection::printCollection()
{
    for(int i = 0; i < _collection.size(); i++)
    {
        std::cout << _collection[i]->toString().c_str() << std::endl;
    }
}

void DataCollection::addToCollection(DataEntry *data)
{
    _collection.push_back(data);
}

bool DataCollection::getDataSegment(int startIndex, int size, std::vector<DataEntry*> &output) const
{
    if((startIndex + size < _collection.size()) && size != 0)
    {
        for(int i = startIndex; i < startIndex + size; i++)
        {
            output.push_back(_collection.at(i));
        }
        return true;
    }
    else
        return false;
}
