#ifndef DATASEGMENT_H
#define DATASEGMENT_H

#include "dataentry.h"

class DataSegment
{
public:
    DataSegment();
    DataSegment(std::vector<DataEntry*> entries);

    std::vector<DataEntry*> getSegments() const;
    DataEntry* getSegment(int index) const;
    std::vector<double> getDataOfType(DataType type) const;

    double getTotalAccelerometer() const;
    double getTotalGyroscope() const;
    double getTotalMagnetometer() const;
    double getTotalBarometer() const;

    void addToSegment(DataEntry* entry);
    void create();

private:
    std::vector<DataEntry*> _segment;

    double _totalAccelerometer;
    double _totalGyroscope;
    double _totalMagnetometer;
    double _totalBarometer;


    double calculateTotal(std::vector<double> values);
};

#endif // DATASEGMENT_H
