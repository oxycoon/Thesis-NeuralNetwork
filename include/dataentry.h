#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <vector>
#include "exercise.h"

enum DataType
{
    NO_DATA
};

class DataEntry
{
public:
    DataEntry(int size, Exercise ex);
    DataEntry(std::vector<double> &e, Exercise ex);
    ~DataEntry();

    std::vector<double>     getEntries() const;
    double                  getEntry(const int index) const;

    void                    modifyEntry(double entry, DataType type, const int index);

private:
    std::vector<double>     _entries;
    std::vector<DataType>   _types;
    int                     _size;
    Exercise                _excercise;

    void                    initializeEntries();
    void                    initializeTypes();
};

#endif // DATAENTRY_H
