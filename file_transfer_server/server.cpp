#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <fstream>
#include <boost/asio.hpp>
#define BUFSIZE 1024

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
    public:
        session(tcp::socket socket, const int id)
        : sock(std::move(socket)), file(std::to_string(id) + ".txt", std::ofstream::out) {}
        void start()
        {
            read();
        }
    private:
        void read()
        {
            auto self(shared_from_this());
            sock.async_read_some(boost::asio::buffer(buf, BUFSIZE),
            [this, self](boost::system::error_code ec, size_t size) {
                if (ec == boost::asio::error::eof) {
                    file.write(buf, size);
                }else if (!ec) {
                    file.write(buf, size);
                    read();
                }
            });
        }
        tcp::socket sock;
        char buf[BUFSIZE];
        std::ofstream file;
};

class server
{
    public:
        server(boost::asio::io_context& io_context, short port) : acc(io_context, tcp::endpoint(tcp::v4(), port))
        {
            accept();
        }
    private:
        void accept()
        {
            static int count = 0;
            acc.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<session>(std::move(socket), ++count)->start();
                }
            accept();
            });
        }
        tcp::acceptor acc;
};

int main()
{
    try {
        boost::asio::io_context io_context;
        server s(io_context, 8080);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

  return 0;
}
