#include "../include/csvreader.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>



//================================================
//                      DataSet
//================================================

DataSet::DataSet()
{

}

DataSet::~DataSet()
{
    _trainingSet.clear();;
    _testingSet.clear();
    _validationSet.clear();
}

void DataSet::clear()
{
    _trainingSet.clear();;
    _testingSet.clear();
    _validationSet.clear();
}

//================================================
//                      CSVReader
//================================================

CSVReader::CSVReader()
{
}

CSVReader::~CSVReader()
{

}

int CSVReader::getNumberDataSet() const
{
    return _numberDataSet;
}

DataSet *CSVReader::getDataSet()
{
    return &_dataSet;
}

/**
 * @brief CSVReader::readCSVFile
 * @param path
 * @param numberInput
 * @param numberOutput
 * @return
 *
 *  Reads a .csv file with {numberInput} input variables and {numberOutput} output variables.
 */
bool CSVReader::readCSVFile(const char *path, int numberInput, int numberOutput, char* separator)
{
    clearData();

    _numberInput = numberInput;
    _numberOutput = numberOutput;
    _separator = separator;

    _numberDataSet = 1;

    std::fstream file;
    file.open(path, std::ios::in);

    if(file.is_open())
    {
        std::string line = "";

        while(!file.eof())
        {
            std::getline(file, line);

            if(line.length() > 2)
            {
                if(line.at(0) == '#') //Checks if the line is a comment.
                {
                    continue;
                }
                else
                {
                    readLine(line);
                }
            }
        }

        //shuffles the given data
        std::random_shuffle(_data.begin(), _data.end());

        //splits data
        _trainingDataEnd = (int) (0.6 * _data.size());
        int testSize = (int)(ceil(0.2 * _data.size()));
        //int validSize = (int)(_data.size() - _trainingDataEnd - testSize);

        //Create training set
        for(int i = 0; i < _trainingDataEnd; i++)
        {
            _dataSet._trainingSet.push_back(_data[i]);
        }

        //Create testing set
        for(int i = _trainingDataEnd; i < _trainingDataEnd + testSize; i++)
        {
            _dataSet._testingSet.push_back(_data[i]);
        }

        //Create validation set
        for(int i = _trainingDataEnd + testSize; i < _data.size(); i++)
        {
            _dataSet._validationSet.push_back(_data[i]);
        }

        std::cout << "File successfully read, patterns found: " << _data.size() << std::endl;

        file.close();

        return true;
    }
    else
        return false;
}

/**
 * @brief CSVReader::clearData
 *
 *  Clears any existing data.
 */
void CSVReader::clearData()
{
    for(int i = 0; i < (int)_data.size(); i++)
    {
        delete _data[i];
    }
    _data.clear();
    _dataSet.clear();
}

/**
 * @brief CSVReader::readLine
 * @param line
 *
 *  Reads the line and pushes the resulting data entry into the
 *  data set.
 */
void CSVReader::readLine(const std::string &line)
{
    std::vector<double> pattern(_numberInput);
    std::vector<double> target(_numberOutput);
    //std::vector<double> target(5);  // Spesific for this task, hack to make
                                    // system work

    char* cstr = new char[line.size() + 1];
    char* token;

    std::strcpy(cstr, line.c_str());

    int i = 0;
    token = std::strtok(cstr, _separator);

    while(token != NULL && i < (_numberInput + _numberOutput))
    {
        if(i < _numberInput)
        {
            pattern[i] = std::atof(token);
        }
        else
        {
            target[i - _numberInput] = std::atof(token);
            //target[std::atof(token)] = 1.0;
        }

        //Move forward
        token = std::strtok(NULL, _separator);
        i++;
    }

    //PRINT FOR DEBUGGING
    std::cout << "pattern: [";
    for (int i=0; i < _numberInput; i++)
    {
        std::cout << pattern[i] << ",";
    }

    std::cout << "] target: [";
    for (int i = 0; i < _numberOutput; i++)
    {
        std::cout << target[i] << ",";
    }
    std::cout << "]" << std::endl;


    _data.push_back(new DataEntry(pattern, target));
}




