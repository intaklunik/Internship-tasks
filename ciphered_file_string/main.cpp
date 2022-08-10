#include <iostream>
#include "Cipher.h"
#include <vector>

int main(int argc, char *argv[])
{
    size_t filesNumber = argc - 2;
    std::string key = argv[1];
    std::vector<std::shared_ptr<File>> files;

    for(size_t i = 0; i < filesNumber; ++i) {
        try{
            files.push_back(std::make_shared<File>(std::string(argv[i + 2])));
        } catch (const std::ifstream::failure &e) {
            std::cerr << "Open file is failure: " << e.what() << std::endl;
        } catch (const std::bad_alloc &e) {
            std::cerr << "Bad_alloc detected: " << e.what();
        }
    }

    if (files.size() == 0) {
        std::cout << "No files" << std::endl;
        return 0;
    }
    try{
        std::unique_ptr<IFile> f = std::make_unique<Cipher>(files.at(0), key);
        std::cout << f->ReadString();
        f->WriteString("de");
    } catch (const std::ifstream::failure &e) {
            std::cerr << "Read/write file is failure: " << e.what() << std::endl;
    } catch (const std::bad_alloc &e) {
            std::cerr << "Bad_alloc detected: " << e.what();
    }


    return 0;
}
