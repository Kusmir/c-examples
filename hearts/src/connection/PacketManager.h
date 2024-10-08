#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H
#include <deque>
#include <memory>

#include "packets/Packet.h"


class PacketManager {
public:
    void append(std::shared_ptr<Packet> packet);
    void prepend(std::shared_ptr<Packet> packet);
    std::shared_ptr<Packet> get_packet();
    [[nodiscard]] bool has_pending_packets() const;
    void process(const char *data, size_t length);

private:
    static constexpr size_t MAX_MESSAGE_SIZE = 256;
    std::deque<std::shared_ptr<Packet>> packets_;
    std::string incomplete_incoming_message_;
};



#endif //PACKETMANAGER_H
