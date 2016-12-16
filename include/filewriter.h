#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>

class FileWriter
{
public:
    FileWriter();


    bool writeFile(std::string fileName, std::string contents, std::string folder = "./");
};

#endif // FILEWRITER_H
