#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <vector>

#include "dataentry.h"
#include "exercise.h"
#include "datasegment.h"


class DataCollection
{
public:
    DataCollection(Exercise ex);
    ~DataCollection();

    DataEntry*                  getEntry(const int index) const;
    int                         getCollectionSize() const;
    int                         getTrainingSetSize() const;
    int                         getTestSetSize() const;
    int                         getValidationSetSize() const;
    Exercise                    getExercise() const;
    void                        setExercise(const Exercise ex);
    std::vector<double>         getTarget() const;
    std::vector<DataSegment>    getValidationSet() const;
    std::vector<DataSegment>    getTestSet() const;
    std::vector<DataSegment>    getTrainingSet() const;

    void                        printCollection();

    void                        createTrainingTestValidationSets(int setSize, double trainingSize, double testSize);
    void                        addToCollection(DataEntry* data);

private:
    std::vector<DataEntry*>     _collection;
    std::vector<DataSegment>    _trainingSet;
    std::vector<DataSegment>    _testSet;
    std::vector<DataSegment>    _validationSet;

    Exercise                    _exercise;
    int                         _trainingSetSize;
    int                         _testSetSize;
    int                         _validationSetSize;

    bool                        getDataSegment(int startIndex, int size, DataSegment &output) const;
};

#endif // DATACOLLECTION_H
