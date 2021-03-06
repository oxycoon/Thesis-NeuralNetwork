#include "include/datacollection.h"

#include <iostream>
#include <algorithm>

DataCollection::DataCollection()
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

std::vector<DataSegment> DataCollection::getTestSet() const
{
    return _testSet;
}

std::vector<DataSegment> DataCollection::getTrainingSet() const
{
    return _trainingSet;
}

std::vector<DataSegment> DataCollection::getFullSet() const
{
    return _fullSet;
}

std::string DataCollection::getName() const
{
    return _name;
}

void DataCollection::setName(const std::string name)
{
    _name = name;
}

void DataCollection::printCollection()
{
    for(int i = 0; i < _collection.size(); i++)
    {
        std::cout << _collection[i]->toString().c_str() << std::endl;
    }
}

void DataCollection::createTrainingTestSets(int setSize, double trainingSize)
{
    int size = _collection.size()-setSize;

    std::vector<DataSegment> temp(size);

    for(int i = 0; i < temp.size(); i++)
    {
        DataSegment tmp;
        getDataSegment(i, setSize, tmp);
        tmp.create();
        temp[i] = tmp;
    }

    std::random_shuffle(temp.begin(), temp.end());

    int trainingDataEnd = (int)((trainingSize*0.01) * temp.size());

    for(int i = 0; i < trainingDataEnd; i++)
    {
        _trainingSet.push_back(temp[i]);
        _fullSet.push_back(temp[i]);
    }

    for(int i = trainingDataEnd; i < temp.size(); i++)
    {
        _testSet.push_back(temp[i]);
        _fullSet.push_back(temp[i]);
    }

    _trainingSetSize = _trainingSet.size();
    _testSetSize = _testSet.size();

    int numFall, numNonFall;

    for(int i = 0; i < _trainingSetSize; i++)
    {
        if(_trainingSet[i].getTarget(0) == 0)
        {
            numFall++;
        }
        else
        {
            numNonFall++;
        }
    }

    QString msg = "Training and testing sests successfully created.\n";
    msg.append("--Training sets: " + QString::number(_trainingSetSize) + "; "
               + QString::number(numFall) + " fall cases, " + QString::number(numNonFall)
               + " non fall cases.\n");
    msg.append("--Test sets: " + QString::number(_testSetSize) + "\n\n");
    emit signDataCollectionConsoleOutput(msg);
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
