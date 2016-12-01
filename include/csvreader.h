#ifndef CSVREADER_H
#define CSVREADER_H

#include <vector>
#include <string>

#include <QtCore>

#include "include/dataentry.h"
#include "include/datacollection.h"

#include "include/enum/exercise.h"

/**
 * @brief The DataSet class
 *
 *  Container class for the data sets.
 */
class DataSet{

public:
    std::vector<DataEntry*> _trainingSet;
    std::vector<DataEntry*> _testingSet;
    std::vector<DataEntry*> _validationSet;

    DataSet();
    ~DataSet();

    void clear();
};

/**
 * @brief The CSVReader class
 *
 *  Class to read a CSV file
 */
class CSVReader : public QObject
{
    Q_OBJECT
public:
    CSVReader();
    ~CSVReader();

    int getNumberDataSet() const;
    DataSet* getDataSet();
    void        enableBarometer(bool enable);

    bool readCSVFile(const char* path, int entries, char* separator);
    bool readCSVFile(const char* path, int entries, char* separator, Exercise ex, DataCollection &output);
    void clearData();

private:
    std::vector<DataEntry*> _data;
    DataSet _dataSet;

    int _numberDataSet;

    int _trainingDataEnd;
    int _numEntries;

    bool _enableBarometer; //disabled by defalt

    char* _separator;

    DataEntry* readLine(const std::string &line, Exercise ex);

signals:
    void signFileReadComplete(const QString &message);

};

#endif // CSVREADER_H
