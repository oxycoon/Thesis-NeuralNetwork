#ifndef DATATYPE_H
#define DATATYPE_H

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
    NODATA,
    UK,             // Unknown data or Unknown Network type
    GYRO,           // Network Type
    ACCELEROMETER,  // Network Type
    BAROMETER,      // Network Type
    COMPASS         // Network Type
};

#endif // DATATYPE_H
