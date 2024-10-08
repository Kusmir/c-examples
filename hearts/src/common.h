#ifndef COMMON_H
#define COMMON_H

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <vector>

#include "connection/Connection.h"
#include "GameEngine/Seat.h"

template<class T>
std::ostream & operator<<(std::ostream &os, std::vector<T> vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        os << *it;
        if (it != prev(vec.end()))
            os << ", ";
    }
    return os;
}

template<class T>
std::ostream & operator<<(std::ostream &os, std::vector<std::vector<T>> vec) {
    for (std::vector<T> x : vec) {
        os << x << '\n';
    }
    return os;
}

std::string current_timestamp_string();
void report_packet(
    Connection &conn, const std::shared_ptr<Packet> &packet, bool sent);

// If after reading that many bytes the message did not end
// (no "\r\n") treat it as invalid.
constexpr size_t MAX_MESSAGE_SIZE = 256;
constexpr size_t BUFFER_SIZE = 256;
constexpr size_t QUEUE_LENGTH = 5; //TODO: ????
constexpr int STDIN = 0;

#endif //COMMON_H
