#ifndef DATASEGMENT_H
#define DATASEGMENT_H

#include "dataentry.h"

class DataSegment
{
public:
    DataSegment();
    DataSegment(std::vector<DataEntry*> entries, std::vector<double> targets);

    void                    setTargets(const std::vector<double> targets);
    void                    setTarget(const int index, const double target);
    void                    addToTarget(const double target);

    std::vector<DataEntry*> getSegments() const;
    DataEntry*              getSegment(int index) const;
    std::vector<double>     getDataOfType(DataType type, bool getTotal) const;
    std::vector<double>     getTargets() const;
    double                  getTarget(int index) const;
    double                  getTotalAccelerometer() const;
    double                  getTotalGyroscope() const;
    double                  getTotalMagnetometer() const;
    double                  getTotalBarometer() const;
    int                     getSegmentSize() const;

    void                    addToSegment(DataEntry* entry);
    void                    create();
    void                    createNoise();

private:
    std::vector<DataEntry*> _segment;
    std::vector<double>     _targets;

    double _totalAccelerometer;
    double _totalGyroscope;
    double _totalMagnetometer;
    double _totalBarometer;

    double calculateTotal(std::vector<double> values);
};

#endif // DATASEGMENT_H
