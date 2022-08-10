#ifndef FILE_H
#define FILE_H

#include <fstream>

class File
{
    public:
        File(const std::string &filepath);
        std::string ReadString();
        void WriteString(const std::string &s);
        ~File(){}
    private:
        std::ifstream input;
        std::ofstream output;
};


#endif // FILE_H
