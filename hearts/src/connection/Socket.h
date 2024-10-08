#ifndef SOCKET_H
#define SOCKET_H
#include "IPEndpoint.h"


class Socket {
public:
    Socket(int fd, IPVersion ip_version);
    explicit Socket(IPVersion ip_version);

    int Close();
    int Connect(IPEndpoint endpoint);
    void Bind(IPEndpoint endpoint);
    void Bind(sockaddr *address, size_t length);
    void Listen(int backlog = 5);
    Socket Accept(IPEndpoint &endpoint);
    ssize_t Send(const void *data, size_t length);
    ssize_t Recv(void *buffer, size_t length);
    void set_blocking(bool blocking);
    bool set_option(int option, bool value);
    int fd() const;
    IPVersion ip_version() const;

private:
    int socket_fd_;
    IPVersion ip_version_;
};



#endif //SOCKET_H
