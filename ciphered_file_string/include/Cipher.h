#ifndef CIPHER_H
#define CIPHER_H
#include <memory>
#include "File.h"

class IFile
{
    public:
        virtual std::string ReadString() = 0;
        virtual void WriteString(const std::string &s) = 0;
};

class Cipher : public IFile
{
    public:
        Cipher(std::shared_ptr<File> &file, const std::string &key) : file(file), key(key) {}
        std::string ReadString();
        void WriteString(const std::string &s);
        ~Cipher() {}
    private:
        std::string cipher(const std::string &s);
        std::string decipher(const std::string &s);
    private:
        std::shared_ptr<File> file;
        const std::string key;
};


#endif // CIPHER_H
