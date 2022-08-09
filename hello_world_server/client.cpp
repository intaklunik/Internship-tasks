#include <iostream>
#include <limits>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int start(int fd)
{
    bool is_running = true;
    const size_t BUFSIZE = 120;
    char buf[BUFSIZE] = {0};
    int size = 0;
    while(is_running) {
        std::cout << "Enter your message: ";
        std::cin.getline(buf, BUFSIZE);
        if ((size = write(fd, buf, std::cin.gcount())) <= 0)
            break;
        if ((size = read(fd, buf, BUFSIZE)) <= 0)
            break;
        std::cout << "Message from server: " << buf << std::endl;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return size;
}

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    const int PORT = 8080;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        std::cerr << "Connect failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    int size = start(sock);
    if (size == -1) {
        std::cerr << "Interaction with server failed" << std::endl;
        exit(EXIT_FAILURE);
    } else if(size == 0) {
        std::cerr << "Interrupt connection" << std::endl;
    }

    return 0;
}
