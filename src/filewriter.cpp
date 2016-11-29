#include "include/filewriter.h"

#include <cstring>
#include <fstream>
#include <iostream>


FileWriter::FileWriter()
{

}

bool FileWriter::writeFile(std::string fileName, std::string contents)
{
    std::ofstream stream;

    stream.open(fileName);
    stream << contents;
    stream.close();

    return true;
}
