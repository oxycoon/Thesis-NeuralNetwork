#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <vector>
#include "exercise.h"

enum DataType
{
    ACCEL_X,
    ACCEL_Y,
    ACCEL_Z,
    COM_X,
    COM_Y,
    COM_Z,
    GYR_X,
    GYR_Y,
    GYR_Z,
    BAR,
    NO_DATA
};

class DataEntry
{
public:
    DataEntry(int size);
    DataEntry(std::vector<double> &e, std::vector<DataType> &type);
    ~DataEntry();

    std::vector<double>     getEntries() const;
    double                  getEntry(const int index) const;
    int                     getSize() const;

    void                    modifyEntry(double entry, DataType type, const int index);
    void                    setTimestamp(long time);

private:
    std::vector<double>     _entries;
    std::vector<DataType>   _types;
    int                     _size;
    long                    _timestamp;

    void                    initializeEntries();
    void                    initializeTypes();
};

#endif // DATAENTRY_H
