#ifndef CSVREADER_H
#define CSVREADER_H

#include <vector>
#include <string>


#include "dataentry.h"
#include "exercise.h"
#include "datacollection.h"

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
class CSVReader
{
public:
    CSVReader();
    ~CSVReader();

    int getNumberDataSet() const;
    DataSet* getDataSet();

    bool readCSVFile(const char* path, int entries, char* separator);
    bool readCSVFile(const char* path, int entries, char* separator, DataCollection &output);
    void clearData();

private:
    std::vector<DataEntry*> _data;
    DataSet _dataSet;

    int _numberDataSet;

    int _trainingDataEnd;
    int _numEntries;

    char* _separator;

    DataEntry* readLine(const std::string &line);
};

#endif // CSVREADER_H
