#ifndef DATAENTRYTOTAL_H
#define DATAENTRYTOTAL_H

#include "dataentry.h"

class DataEntryTotal
{
public:
    DataEntryTotal(std::vector<DataEntry*> entries);

    double getTotalAccelerometer();
    double getTotalGyroscope();
    double getTotalMagnetometer();
    double getTotalBarometer();

private:
    double _totalAccelerometer;
    double _totalGyroscope;
    double _totalMagnetometer;
    double _totalBarometer;

    void create(std::vector<DataEntry*> entries);
    double calculateTotal(std::vector<double> values);
};

#endif // DATAENTRYTOTAL_H
