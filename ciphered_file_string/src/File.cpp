#include "File.h"
#include <iostream>

File::File(const std::string &filepath)
{
    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    output.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try {
        input.open(filepath);
        output.open(filepath, std::ios::app);
    } catch(const std::ifstream::failure &e) {
        throw;
    }
}

std::string File::ReadString()
{
    std::string line;
    try{
        std::getline(input, line);
    } catch(const std::ifstream::failure &e) {
        if (input.eof()) {
            std::cout << "EOF" << std:: endl;
        } else
            throw;
    }

    return line;
}

void File::WriteString(const std::string &s)
{
    try {
        output << s;
        output.flush();
    } catch(const std::ifstream::failure &e) {
        throw;
    }

}
