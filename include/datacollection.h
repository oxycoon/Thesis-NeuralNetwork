#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <vector>

#include "dataentry.h"
#include "exercise.h"


class DataCollection
{
public:
    DataCollection(Exercise ex);
    ~DataCollection();

    DataEntry*                  getEntry(const int index) const;
    int                         getCollectionSize() const;
    Exercise                    getExercise() const;
    void                        setExercise(const Exercise ex);

    void                        printCollection();

    void                        addToCollection(DataEntry* data);
    bool                        getDataSegment(int startIndex, int size, std::vector<DataEntry*> &output) const;
private:
    std::vector<DataEntry*>     _collection;
    Exercise                    _exercise;
};

#endif // DATACOLLECTION_H
