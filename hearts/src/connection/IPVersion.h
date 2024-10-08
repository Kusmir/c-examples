#ifndef IPVERSION_H
#define IPVERSION_H
#include <ostream>
#include <sys/socket.h>

enum IPVersion {
    UNKNOWN = AF_UNSPEC,
    IPv4 = AF_INET,
    IPv6 = AF_INET6
};

inline std::ostream &operator << (std::ostream &os, IPVersion ip_version) {
    switch (ip_version) {
        case IPv4:
            return os << "IPv4";
        case IPv6:
            return os << "IPv6";
        default:
            return os << "unknown ip version";
    }
}

#endif //IPVERSION_H
