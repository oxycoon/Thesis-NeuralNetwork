#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <vector>

#include "dataentry.h"
#include "exercise.h"


class DataCollection
{
public:
    DataCollection(Exercise ex);

    DataEntry*                  getEntry(int index) const;
    int                         getCollectionSize() const;
    Exercise                    getExercise() const;

    void                        addToCollection(DataEntry* data);
    bool                        getDataSegment(int startIndex, int size, std::vector<DataEntry*> &output);

private:
    std::vector<DataEntry*>     _collection;
    Exercise                    _exercise;
};

#endif // DATACOLLECTION_H
