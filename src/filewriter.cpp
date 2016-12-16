#include "include/filewriter.h"

#include <cstring>
#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>

FileWriter::FileWriter()
{

}

bool FileWriter::writeFile(std::string fileName, std::string contents, std::string folder)
{
    boost::filesystem::path dir(folder);

    if(!(boost::filesystem::exists(dir)))
    {
        if (boost::filesystem::create_directory(dir));
    }

    std::ofstream stream;

    std::string fullName = folder + fileName;

    stream.open(fullName);
    stream << contents;
    stream.close();

    return true;
}
