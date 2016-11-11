#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <vector>

#include "dataentry.h"
#include "exercise.h"
#include "datasegment.h"


class DataCollection
{
public:
    DataCollection();
    ~DataCollection();

    DataEntry*                  getEntry(const int index) const;
    int                         getCollectionSize() const;
    int                         getTrainingSetSize() const;
    int                         getTestSetSize() const;
    Exercise                    getExercise() const;
    void                        setExercise(const Exercise ex);
    std::vector<DataSegment>    getTestSet() const;
    std::vector<DataSegment>    getTrainingSet() const;

    void                        printCollection();

    void                        createTrainingTestSets(int setSize, double trainingSize);
    void                        addToCollection(DataEntry* data);
    //void                        addCollection(DataCollection &collection);

private:
    std::vector<DataEntry*>     _collection;
    std::vector<DataSegment>    _trainingSet;
    std::vector<DataSegment>    _testSet;

    int                         _trainingSetSize;
    int                         _testSetSize;

    bool                        getDataSegment(int startIndex, int size, DataSegment &output) const;
};

#endif // DATACOLLECTION_H
