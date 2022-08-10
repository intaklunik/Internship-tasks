#include "Cipher.h"

 std::string Cipher::cipher(const std::string &s)
 {
    std::string currKey = key;
    std::string result = s;

    while (currKey.size() < result.size())
        currKey += key;
    for (std::string::size_type i = 0; i < result.size(); ++i)
        result[i] ^= currKey[i];

    return result;
 }

std::string Cipher::decipher(const std::string &s)
{
    return cipher(s);
}

std::string Cipher::ReadString()
{
    return decipher(file->ReadString());
}

void Cipher::WriteString(const std::string &s)
{
    file->WriteString(cipher(s));
}
