#include "PacketManager.h"
#include "../utils/parser.h"
#include "../exceptions.hpp"

#include <vector>

void PacketManager::append(std::shared_ptr<Packet> packet) {
    packets_.emplace_back(std::move(packet));
}

void PacketManager::prepend(std::shared_ptr<Packet> packet) {
    packets_.emplace_front(std::move(packet));
}

std::shared_ptr<Packet> PacketManager::get_packet() {
    auto packet = packets_.front();
    packets_.pop_front();
    return packet;
}

bool PacketManager::has_pending_packets() const {
    return !packets_.empty();
}

void PacketManager::process(const char *data, size_t length) {
    std::vector<std::string> messages = parser::split_messages(data, length);
    for (const std::string &message: messages) {
        incomplete_incoming_message_ += message;
        if (parser::is_message_complete(incomplete_incoming_message_)) {
            packets_.emplace_back(parser::parse(message));
            incomplete_incoming_message_ = "";
        }
    }

    if (!messages.empty() and !parser::is_message_complete(messages.back())) {
        incomplete_incoming_message_ += messages.back();
    }

    // No packet should be that long, we deal with incorrect client
    // and should abort the connection.
    if (incomplete_incoming_message_.length() > MAX_MESSAGE_SIZE)
        throw InvalidMessageException(incomplete_incoming_message_);
}
