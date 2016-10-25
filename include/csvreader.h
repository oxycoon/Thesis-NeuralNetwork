#ifndef CSVREADER_H
#define CSVREADER_H

#include <vector>
#include <string>


#include "dataentry.h"

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
 *  Class to read a CSV file with patterns and targets. .csv file format:
 *
 *      Input values | Result values
 *
 *  Specify the amount of input values, output values and the seperator in constructor.
 */
class CSVReader
{
public:
    CSVReader();
    ~CSVReader();

    int getNumberDataSet() const;
    DataSet* getDataSet();

    bool readCSVFile(const char* path, int numberInput, int numberOutput, char* separator);
    void clearData();

private:
    std::vector<DataEntry*> _data;
    DataSet _dataSet;

    int _numberInput;
    int _numberOutput;
    int _numberDataSet;

    int _trainingDataEnd;

    char* _separator;

    void readLine(const std::string &line);
};

#endif // CSVREADER_H
