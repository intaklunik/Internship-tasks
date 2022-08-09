#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>


int interact_with_client(int fd)
{
    const char msg[] = "hello";
    const char answ[] = "world";
    const int BUFSIZE = 120;
    char buf[BUFSIZE] = {0};
    int size = read(fd, buf, BUFSIZE);
    if (size <= 0)
        return size;
    if (strcmp(msg, buf) == 0) {
        size = write(fd, answ, strlen(answ) + 1);
    } else {
        size = write(fd, buf, strlen(buf) + 1);
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
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(sock, 10) == -1) {
        std::cerr << "Listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    int epollfd = epoll_create(1);
    if(epollfd == -1) {
        std::cerr << "Epoll failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    const int MAXEVENTS = 50;
    struct epoll_event ev;
    struct epoll_event events[MAXEVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        std::cerr << "Epoll_ctl failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    int nfds = 0;
    bool is_running = true;

    while(is_running) {
        nfds = epoll_wait(epollfd, events, MAXEVENTS, 2000);
        if (nfds == 0)
            continue;
        else if (nfds == -1) {
            std::cerr << "Epoll_wait failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < nfds; ++i) {
            if(events[i].data.fd == sock) {
                int client = accept(sock, NULL, NULL);
                if (client == -1) {
                    std::cerr << "Accept failed" << std::endl;
                    exit(EXIT_FAILURE);
                }
                ev.events  = EPOLLIN;
                ev.data.fd = client;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client, &ev) == -1) {
                    std::cerr << "Epoll_ctl failed" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else {
                int size = interact_with_client(events[i].data.fd);
                if (size == -1) {
                        std::cerr << "Interaction with client failed" << std::endl;
                        break;
                } else if (size == 0) {
                    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, ev.data.fd, NULL) == -1) {
                        std::cerr << "Epoll_ctl failed" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    close(ev.data.fd);
                }
            }
        }
    }

    close(epollfd);
    close(sock);

    return 0;
}
