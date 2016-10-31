#include "../include/datacollection.h"

DataCollection::DataCollection(Exercise ex = Exercise::UNKNOWN):
    _exercise(ex)
{

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

void DataCollection::addToCollection(DataEntry *data)
{
    _collection.push_back(data);
}

bool DataCollection::getDataSegment(int startIndex, int size, std::vector<DataEntry*> &output)
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
