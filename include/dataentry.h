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
    NO_DATA,
    UK,
    GYRO,
    ACCELEROMETER,
    BAROMETER,
    COMPASS
};

class DataEntry
{
public:
    DataEntry(int size);
    DataEntry(std::vector<double> &e, std::vector<DataType> &type, Exercise ex);
    ~DataEntry();

    std::vector<double>     getEntries() const;
    std::vector<double>     getEntriesOfDataType(DataType type) const;
    double                  getEntry(const int index) const;
    DataType                getDataType(const int index) const;
    Exercise                getExercise() const;
    int                     getSize() const;

    void                    modifyEntry(double entry, DataType type, const int index);
    void                    setTimestamp(long time);
    void                    setExercise(Exercise ex);

    std::string             toString();

private:
    std::vector<double>     _entries;
    std::vector<DataType>   _types;
    int                     _size;
    long                    _timestamp;
    Exercise                _exercise;

    void                    initializeEntries();
    void                    initializeTypes();
};

#endif // DATAENTRY_H
