#ifndef INVALIDPACKET_H
#define INVALIDPACKET_H
#include "Packet.h"

#include <string>

class InvalidPacket : public Packet {
public:
    [[nodiscard]] std::string get_message() const override {
        return message_ + "\r\n";
    }

    explicit InvalidPacket(std::string message)
        : message_(std::move(message)){
        type_ = INVALID_PACKET;
    }

private:
    std::string message_;
};

#endif //INVALIDPACKET_H
