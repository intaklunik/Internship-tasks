#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#define BUFSIZE 1024

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try {
    if (argc != 2) {
      std::cerr << "No filepath" << std::endl;
      return -1;
    }
    std::ifstream file(argv[1], std::ifstream::in);
    if (file.fail()) {
        std::cerr << "No file" << std::endl;
        return -1;
    }

    boost::asio::io_context io_context;
    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("127.0.0.1", "8080"));
    char buf[BUFSIZE];
    while (!file.eof()) {
        file.read(buf, BUFSIZE);
        boost::asio::write(s, boost::asio::buffer(buf, file.gcount()));
    }
  } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
