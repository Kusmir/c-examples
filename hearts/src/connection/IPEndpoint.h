#ifndef IPENDPOINT_H
#define IPENDPOINT_H

#include "IPVersion.h"

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>


class IPEndpoint {
public:
    IPEndpoint(const char * ip, uint16_t port, IPVersion = UNKNOWN);
    IPEndpoint(sockaddr *addr);
    IPEndpoint() {};
    [[nodiscard]] IPVersion ip_version() const;
    [[nodiscard]] std::string ip_string() const;
    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] uint16_t port() const;
    [[nodiscard]] sockaddr_in sockaddr4() const;
    [[nodiscard]] sockaddr_in6 sockaddr6() const;

    friend std::ostream & operator<<(std::ostream &os, const IPEndpoint &obj) {
        return os << obj.to_string();
    }

private:
    uint16_t port_;
    IPVersion ip_version_;

    // That's a little bit hacky. I don't want to store pointer to sockaddr
    // to avoid issues with memory allocation, so we store it as an array
    // of bytes and cast it properly when needed.
    std::vector<uint8_t> sockaddr_bytes_;
};



#endif //IPENDPOINT_H
