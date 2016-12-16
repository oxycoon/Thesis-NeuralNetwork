#include "include/csvreader.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

#include <boost/regex.hpp>

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

CSVReader::CSVReader(): _enableBarometer(false)
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

void CSVReader::enableBarometer(bool enable)
{
    _enableBarometer = enable;
}

/**
 * @brief CSVReader::readCSVFile
 * @param path
 *      Path for file
 * @param entries
 *      Number of entries in a line
 * @param separator
 *      Separator used in the .csv file
 * @return
 *      Success or failure of the parsing
 *
 *  Reads a .csv file with {numberInput} input variables and {numberOutput} output variables.
 */
bool CSVReader::readCSVFile(const char *path, int entries, const char* separator)
{
    clearData();

    _separator = separator;
    _numEntries = entries;

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
                    readLine(line, Exercise::UNKNOWN);
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
 * @brief CSVReader::readCSVFile
 * @param path
 *      Path for file
 * @param entries
 *      Number of entries in a line
 * @param separator
 *      Separator used in the .csv file
 * @param output
 *      Output reference to put the results
 * @return
 *      Success or failure of the parsing
 *
 *  Reads a .csv file with {numberInput} input variables and {numberOutput} output variables.
 */
bool CSVReader::readCSVFile(const char *path, int entries,
                            const char* separator, Exercise ex, DataCollection *output)
{
    _separator = separator;
    _numEntries = entries;

    _numberDataSet = 0;

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
                    DataEntry* temp = readLine(line, ex);

                    if(_enableBarometer)
                    {
                        output->addToCollection(temp);
                    }
                    else
                    {
                        if(temp->getDataType(9) != DataType::BAR)
                        {
                            output->addToCollection(temp);
                            _numberDataSet++;
                        }
                        else
                        {
                            delete temp;
                        }
                    }
                }
            }
        }
        QString message = "File " + QString::fromStdString(path) + " successfully read. " + QString::number(_numberDataSet) + " entries.";
        emit signFileReadComplete(message);
        emit signFileReadConsoleOutput(message);
        file.close();
        return true;
    }
    else
    {

        /*QString message = "File " + QString::fromStdString(path) + " not found!";
        emit signFileReadComplete(message);
        emit signFileReadConsoleOutput(message);
        std::cout << message.toStdString() << std::endl;*/

        return false;
    }
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
DataEntry* CSVReader::readLine(const std::string &line, Exercise ex)
{
    std::vector<double> pattern(_numEntries);
    std::vector<DataType> type(_numEntries);
    unsigned long timestamp = 0;

    char* cstr = new char[line.size() + 1];
    char* token;

    std::strcpy(cstr, line.c_str());

    int i = 0;
    token = std::strtok(cstr, _separator);

    while(token != NULL && i < _numEntries+1)
    {
        DataType tempTy = DataType::NODATA;
        if(i == 0)
        {
            timestamp = std::atoll(token);
        }
        else
        {
            std::string temp = token;
            boost::regex expression{"AX|AY|AZ|GX|GY|GZ|CX|CY|CZ|UK|BAR"};
            boost::smatch match;


            if(boost::regex_search(temp, match, expression))
            {
                //std::cout << match[0] << std::endl; //debug
                if(match[0] == "AX")        {tempTy = DataType::ACCEL_X; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "AY")   {tempTy = DataType::ACCEL_Y; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "AZ")   {tempTy = DataType::ACCEL_Z; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "GX")   {tempTy = DataType::GYR_X; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "GY")   {tempTy = DataType::GYR_Y; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "GZ")   {tempTy = DataType::GYR_Z; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "CX")   {tempTy = DataType::COM_X; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "CY")   {tempTy = DataType::COM_Y; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "CZ")   {tempTy = DataType::COM_Z; temp.erase(temp.size()-2, temp.size());}
                else if(match[0] == "BAR")  {tempTy = DataType::BAR; temp.erase(temp.size()-3, temp.size());}
                else if(match[0] == "UK")   {tempTy = DataType::NODATA;}
            }
            type[i-1] = tempTy;
            pattern[i-1] = std::stod(temp);
        }
        //Move forward
        token = std::strtok(NULL, _separator);
        i++;
    }
    return new DataEntry(pattern, type, ex);
}




