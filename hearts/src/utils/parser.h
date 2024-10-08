#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>
#include <string>

#include "../GameEngine/Card.h"
#include "../connection/packets/Packet.h"

namespace parser {
    std::shared_ptr<Packet> parse(std::string message);

    game_engine::Card parse_player_command(std::string command);
    std::vector<std::string> split_messages(char const *message, size_t length);
    bool is_message_complete(const std::string &message);
    std::vector<game_engine::Card> parse_cards(const std::string &s);

}

#endif //PARSER_H
