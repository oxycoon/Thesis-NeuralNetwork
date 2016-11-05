#include "../include/datacollection.h"

#include <iostream>
#include <algorithm>

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

int DataCollection::getTrainingSetSize() const
{
    return _trainingSetSize;
}

int DataCollection::getTestSetSize() const
{
    return _testSetSize;
}

int DataCollection::getValidationSetSize() const
{
    return _validationSetSize;
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

std::vector<DataSegment> DataCollection::getValidationSet() const
{
    return _validationSet;
}

std::vector<DataSegment> DataCollection::getTestSet() const
{
    return _testSet;
}

std::vector<DataSegment> DataCollection::getTrainingSet() const
{
    return _trainingSet;
}

void DataCollection::printCollection()
{
    for(int i = 0; i < _collection.size(); i++)
    {
        std::cout << _collection[i]->toString().c_str() << std::endl;
    }
}

void DataCollection::createTrainingTestValidationSets(int setSize, double trainingSize, double testSize)
{
    std::vector<DataSegment> temp(_collection.size()-setSize);

    for(int i = 0; i < temp.size(); i++)
    {
        DataSegment tmp;
        getDataSegment(i, setSize, tmp);
        temp[i] = tmp;
    }

    std::random_shuffle(temp.begin(), temp.end());

    int trainingDataEnd = (int)(trainingSize * temp.size());
    int test = (int)(ceil(testSize * temp.size()));

    for(int i = 0; i < trainingDataEnd; i++)
    {
        _trainingSet.push_back(temp[i]);
    }

    for(int i = trainingDataEnd; i < trainingDataEnd + test; i++)
    {
        _testSet.push_back(temp[i]);
    }

    for(int i = trainingDataEnd + test; i < temp.size(); i++)
    {
        _validationSet.push_back(temp[i]);
    }

    _trainingSetSize = _trainingSet.size();
    _testSetSize = _testSet.size();
    _validationSetSize = _validationSet.size();
}

void DataCollection::addToCollection(DataEntry *data)
{
    _collection.push_back(data);
}

bool DataCollection::getDataSegment(int startIndex, int size, DataSegment &output) const
{
    if((startIndex + size < _collection.size()) && size != 0)
    {
        for(int i = startIndex; i < startIndex + size; i++)
        {
            output.addToSegment(_collection.at(i));
        }
        return true;
    }
    else
        return false;
}
