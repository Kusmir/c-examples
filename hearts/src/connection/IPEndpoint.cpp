#include "IPEndpoint.h"

#include <cassert>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>

#include "../exceptions.hpp"
#include "../common.h"

IPEndpoint::IPEndpoint(const char *host, uint16_t port, IPVersion ip_version)
    : port_(port) {
    addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = ip_version;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo *address_result;
    int errcode = getaddrinfo(host, NULL, &hints, &address_result);
    if (errcode != 0) {
        throw SysFailed("getaddrinfo");
    }

    assert(address_result->ai_family == AF_INET or
        address_result->ai_family == AF_INET6);
    ip_version_ = (address_result->ai_family == AF_INET
                      ? IPv4
                      : IPv6
    );

    if (ip_version != UNKNOWN) assert(ip_version == ip_version_);

    if (ip_version_ == IPv4) {
        sockaddr_in send_address = {};
        send_address.sin_family = IPv4;
        send_address.sin_addr.s_addr =
                ((sockaddr_in *) (address_result->ai_addr))->sin_addr.s_addr;
        send_address.sin_port = htons(port);

        sockaddr_bytes_.resize(sizeof(sockaddr_in));
        memcpy(&sockaddr_bytes_[0], &send_address, sizeof(send_address));
    } else { //IPv6
        sockaddr_in6 send_address = {};
        send_address.sin6_family = IPv6;
        send_address.sin6_addr =
                ((sockaddr_in6 *) (address_result->ai_addr))->sin6_addr;
        send_address.sin6_port = htons(port);

        sockaddr_bytes_.resize(sizeof(send_address));
        memcpy(&sockaddr_bytes_[0], &send_address, sizeof(send_address));
    }

    freeaddrinfo(address_result);
}

IPEndpoint::IPEndpoint(sockaddr *addr) {
    assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
    if (addr->sa_family == AF_INET) { // IPv4
        sockaddr_in addrv4 = *reinterpret_cast<sockaddr_in *>(addr);
        ip_version_ = IPv4;
        port_ = ntohs(addrv4.sin_port);

        sockaddr_bytes_.resize(sizeof(addrv4));
        memcpy(&sockaddr_bytes_[0], &addrv4, sizeof(addrv4));
    } else { // IPv6
        sockaddr_in6 addrv6 = *reinterpret_cast<sockaddr_in6 *>(addr);
        ip_version_ = IPv6;
        port_ = ntohs(addrv6.sin6_port);

        sockaddr_bytes_.resize(sizeof(addrv6));
        memcpy(&sockaddr_bytes_[0], &addrv6, sizeof(addrv6));
    }
}

IPVersion IPEndpoint::ip_version() const {
    return ip_version_;
}


std::string IPEndpoint::ip_string() const {
    char ip_str[INET6_ADDRSTRLEN];
    if (ip_version_ == IPv4) {
        sockaddr_in addr;
        memcpy(&addr, sockaddr_bytes_.data(), sizeof(sockaddr_in));
        inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    } else if (ip_version_ == IPv6) {
        sockaddr_in6 addr;
        memcpy(&addr, sockaddr_bytes_.data(), sizeof(sockaddr_in6));
        inet_ntop(AF_INET6, &(addr.sin6_addr), ip_str, INET6_ADDRSTRLEN);
    } else {
        throw std::runtime_error("Unsupported IP version");
    }
    return std::string(ip_str);
}

std::string IPEndpoint::to_string() const {
    return ip_string() + ":" + std::to_string(port());
}

uint16_t IPEndpoint::port() const {
    return port_;
}

sockaddr_in IPEndpoint::sockaddr4() const {
    if (ip_version_ != IPv4) {
        throw std::runtime_error("IP version is not IPv4");
    }
    sockaddr_in addr = {};
    memcpy(&addr, sockaddr_bytes_.data(), sizeof(sockaddr_in));
    return addr;
}

sockaddr_in6 IPEndpoint::sockaddr6() const {
    if (ip_version_ != IPv6) {
        throw std::runtime_error("IP version is not IPv6");
    }
    sockaddr_in6 addr = {};
    memcpy(&addr, sockaddr_bytes_.data(), sizeof(sockaddr_in6));
    return addr;
}