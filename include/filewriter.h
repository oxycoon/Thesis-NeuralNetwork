#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>

class FileWriter
{
public:
    FileWriter();


    bool writeFile(std::string fileName, std::string contents);
};

#endif // FILEWRITER_H
