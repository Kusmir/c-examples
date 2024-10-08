#include "Socket.h"

#include <cassert>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "../exceptions.hpp"

Socket::Socket(int fd, IPVersion ip_version)
    : socket_fd_(fd), ip_version_(ip_version) {
}

Socket::Socket(IPVersion ip_version)
    : ip_version_(ip_version) {

    assert(ip_version != UNKNOWN);

    socket_fd_ = socket(ip_version, SOCK_STREAM, 0);
    if (socket_fd_ < 0) {
        throw SysFailed("socket");
    }
}

int Socket::Close() {
    // fake socket
    if (socket_fd_ == -1) return 0;

    int result = close(socket_fd_);
    if (result < 0) {
        throw SysFailed("close");
    }

    return result;
}

int Socket::Connect(IPEndpoint endpoint) {
    int result;
    if (endpoint.ip_version() == IPv4) {
        sockaddr_in addr = endpoint.sockaddr4();
        result = connect(socket_fd_, (sockaddr *) (&addr), sizeof addr);
    } else { // IPv6
        sockaddr_in6 addr = endpoint.sockaddr6();
        result = connect(socket_fd_, (sockaddr *) (&addr), sizeof addr);
    }

    if (result < 0)
        throw SysFailed("connect");
    return result;
}

void Socket::Bind(IPEndpoint endpoint) {
    if (endpoint.ip_version() == IPv4) {
        sockaddr_in addr = endpoint.sockaddr4();
        Bind((sockaddr *) &addr, sizeof(addr));
    } else {
        // IPv6
        sockaddr_in6 addr = endpoint.sockaddr6();
        Bind((sockaddr *) &addr, sizeof(addr));
    }
}

void Socket::Bind(sockaddr *address, size_t length) {
    int result = bind(socket_fd_, address, length);
    if (result < 0)
        throw SysFailed("bind");
}

void Socket::Listen(int backlog) {
    int result = listen(socket_fd_, backlog);
    if (result < 0) {
        throw SysFailed("listen");
    }
}

Socket Socket::Accept(IPEndpoint &endpoint) {
    assert(ip_version_ == IPVersion::IPv4 or ip_version_ == IPVersion::IPv6);

    if (ip_version_ == IPv4) {
        sockaddr_in addr = {};
        socklen_t len = sizeof(sockaddr_in);
        int accepted_fd = accept(socket_fd_, (sockaddr *) (&addr), &len);
        if (accepted_fd < 0) {
            throw SysFailed("accept");
        }

        endpoint = IPEndpoint((sockaddr *) &addr);
        return Socket(accepted_fd, IPv4);
    } else {
        // IPv6
        sockaddr_in6 addr = {};
        socklen_t len = sizeof(sockaddr_in6);
        int accepted_fd = accept(socket_fd_, (sockaddr *) (&addr), &len);
        if (accepted_fd < 0) {
            throw SysFailed("accept");
        }

        endpoint = IPEndpoint((sockaddr *) &addr);
        return Socket(accepted_fd, IPv6);
    }
}

// If send would block, returns < 0.
ssize_t Socket::Send(const void *data, size_t length) {
    ssize_t sent = send(socket_fd_, data, length, 0);
    if (sent < 0 and errno != EAGAIN and errno != EWOULDBLOCK) {
        if (!(errno == EAGAIN or errno == EWOULDBLOCK))
            throw SysFailed("send");
    }

    return sent;
}

ssize_t Socket::Recv(void *buffer, size_t length) {
    ssize_t received = recv(socket_fd_, buffer, length, 0);
    if (received < 0)
        throw SysFailed("recv");
    return received;
}

void Socket::set_blocking(bool blocking) {
    int flags = fcntl(socket_fd_, F_GETFL, 0);
    if (flags < 0)
        throw SysFailed("fcntl");
    flags = blocking
                ? (flags & ~O_NONBLOCK)
                : (flags | O_NONBLOCK);
    int result = fcntl(socket_fd_, F_SETFL, flags);
    if (result < 0)
        throw SysFailed("fcntl");
}

bool Socket::set_option(int option, bool value) {
    return setsockopt(socket_fd_, SOL_SOCKET, option,
                      &value, sizeof(value));
}

int Socket::fd() const {
    return socket_fd_;
}

IPVersion Socket::ip_version() const {
    return ip_version_;
}
