#ifndef DATASEGMENT_H
#define DATASEGMENT_H

#include "include/dataentry.h"

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
    std::vector<double>     getDataOfType(DataType type, bool getTotal=false) const;
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

    /**
     * @brief _targets
     *
     *  The targets for the set. If the set is a timeseries
     *  It has the following structure where 1-9 are the inputs
     *  for the next DataEntry in the collection:
     *      0 - Exercise
     *      1 - Accel_X
     *      2 - Accel_Y
     *      3 - Accel_Z
     *      4 - Gyro_X
     *      5 - Gyro_Y
     *      6 - Gyro_Z
     *      7 - Mag_X
     *      8 - Mag_Y
     *      9 - Mag_Z
     */
    std::vector<double>     _targets;

    double _totalAccelerometer;
    double _totalGyroscope;
    double _totalMagnetometer;
    double _totalBarometer;

    double calculateTotal(std::vector<double> values);
};

#endif // DATASEGMENT_H
