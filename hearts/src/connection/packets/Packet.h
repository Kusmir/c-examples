#ifndef PACKET_H
#define PACKET_H
#include <string>

#include "PacketType.h"


class Packet {
public:
    virtual ~Packet() = default;
    [[nodiscard]] virtual std::string get_message() const = 0;
    [[nodiscard]] PacketType type() const { return type_; }
protected:
    PacketType type_ = INVALID_PACKET;
};



#endif //PACKET_H
