#include "parser.h"

#include <algorithm>

#include "../connection/packets/Packet.h"

#include <cassert>
#include <memory>
#include <utility>
#include <optional>
#include <vector>

#include "../exceptions.hpp"
#include "../common.h"
#include "../connection/packets/PacketsInclude.h"
#include "../GameEngine/Seat.h"
#include "../GameEngine/Card.h"

using pPacket = std::shared_ptr<Packet>;

namespace {
    // Check if next token to read is a card.
    // Returns length of card string or 0 if the token is invalid.
    size_t is_valid_card(const std::string &message, size_t pos) {
        // Check if substring of lenght len is a valid card value.
        auto is_valid_value = [&](size_t len) {
            if (pos + len >= message.size()) return false;
            std::optional<game_engine::Card::Value> value;
            value = game_engine::Card::string_to_value(message.substr(pos, len));
            return value != std::nullopt;
        };

        // Check if character at index is a valid card color.
        auto is_valid_color = [&](size_t index) {
            if (index >= message.size()) return false;
            return game_engine::Card::char_to_color(message[index]) != std::nullopt;
        };

        for (int k: {1, 2})
            if (is_valid_value(k) and is_valid_color(pos + k))
                return k + 1;
        return 0;
    }

    bool is_valid_seat(const std::string &message, size_t pos) {
        if (pos >= message.size()) return false;

        char c = message[pos];
        return c == 'N' or c == 'S' or c == 'E' or c == 'W';
    }

    std::pair<Seat, size_t> parse_seat(
        const std::string &message, size_t pos
    ) {
        if (!is_valid_seat(message, pos))
            throw InvalidMessageException(message);
        return {char_to_seat(message[pos]), pos + 1};
    }

    std::pair<std::vector<Seat>, size_t> parse_seats_list(
        const std::string &message, size_t pos
    ) {
        std::vector<Seat> seats;
        while (pos < message.size() and is_valid_seat(message, pos)) {
            seats.emplace_back(char_to_seat(message[pos++]));
        }

        return {seats, pos};
    }

    std::pair<game_engine::Card, size_t> parse_card(
        const std::string &message, size_t pos
    ) {
        size_t len = is_valid_card(message, pos);
        if(len == 0 or pos + len > message.size())
            throw InvalidMessageException(message);

        std::string value_str = message.substr(pos, len - 1);
        char color_char = message[pos + len - 1];

        auto value = game_engine::Card::string_to_value(value_str);
        auto color = game_engine::Card::char_to_color(color_char);

        assert(value != std::nullopt and color != std::nullopt);

        return {game_engine::Card(value.value(), color.value()), pos + len};
    }

    std::pair<std::vector<game_engine::Card>, size_t> parse_cards_list(
        const std::string &message,
        size_t pos
    ) {
        std::vector<game_engine::Card> cards;
        while (is_valid_card(message, pos)) {
            game_engine::Card card;
            std::tie(card, pos) = parse_card(message, pos);
            cards.emplace_back(card);
        }

        return {cards, pos};
    }

    std::pair<size_t, size_t> parse_number(
        const std::string &message,
        size_t pos, bool ignore_cards = false
    ) {
        std::string number_str = "";
        while (pos < message.size() and
               (ignore_cards or !is_valid_card(message, pos))
               and !is_valid_seat(message, pos)) {
            number_str += message[pos++];
        }

        if (!number_str.empty() and
            std::ranges::all_of(number_str.begin(), number_str.end(), ::isdigit))
            return {std::stoul(number_str), pos};

        throw InvalidMessageException(message + ". Expected number.");
    }

    pPacket parse_packet(
        const std::string &packet_type,
        const std::string &message,
        size_t pos
    ) {
        pPacket packet;
        if (packet_type == "IAM") {
            Seat seat;

            try {
                std::tie(seat, pos) = parse_seat(message, pos);
            } catch (InvalidMessageException &e) {
                return std::make_shared<InvalidPacket>(message);
            }

            packet = std::make_shared<TakePlace>(seat);
        } else if (packet_type == "BUSY") {
            std::vector<Seat> seats;

            try {
                std::tie(seats, pos) = parse_seats_list(message, pos);
            } catch (InvalidMessageException &e) {
                return std::make_shared<InvalidPacket>(message);
            }

            packet = std::make_shared<Busy>(seats);
        } else if (packet_type == "DEAL") {
            size_t rules;
            Seat first_player_position;
            std::vector<game_engine::Card> cards;

            try {
                std::tie(rules, pos) = parse_number(message, pos);
                std::tie(first_player_position, pos) = parse_seat(message, pos);
                std::tie(cards, pos) = parse_cards_list(message, pos);
            } catch (InvalidMessageException &e) {
                return std::make_shared<InvalidPacket>(message);
            }

            packet = std::make_shared<Deal>(rules, first_player_position, cards);
        } else if (packet_type == "TRICK") {
            size_t turn_id;
            std::vector<game_engine::Card> cards;

            try {
                std::tie(turn_id, pos) = parse_number(message, pos);
                std::tie(cards, pos) = parse_cards_list(message, pos);
            } catch (InvalidMessageException &e) {
                return std::make_shared<InvalidPacket>(message);
            }

            packet = std::make_shared<TrickPacket>(turn_id, cards);
        } else if (packet_type == "WRONG") {
            size_t turn_id;

            try {
                std::tie(turn_id, pos) = parse_number(message, pos);
            } catch (InvalidMessageException &e) {
                return std::make_shared<InvalidPacket>(message);
            }

            packet = std::make_shared<Wrong>(turn_id);
        } else if (packet_type == "TAKEN") {
            size_t turn_id;
            std::vector<game_engine::Card> cards;
            Seat seat;

            try {
                std::tie(turn_id, pos) = parse_number(message, pos);
                std::tie(cards, pos) = parse_cards_list(message, pos);
                std::tie(seat, pos) = parse_seat(message, pos);
            } catch (InvalidMessageException &e) {
                return std::make_shared<InvalidPacket>(message);
            }

            packet = std::make_shared<Taken>(turn_id, cards, seat);
        } else if (packet_type == "SCORE" or packet_type == "TOTAL") {
            std::array<size_t, 4> scores = {0, 0, 0, 0};
            std::array<size_t, 4> new_place = {1, 1, 1, 1};

            for (size_t rep = 0; rep < scores.size(); ++rep) {
                Seat seat;
                size_t score;

                try {
                    std::tie(seat, pos) = parse_seat(message, pos);
                    if (!new_place[seat])
                        return std::make_shared<InvalidPacket>(message);

                    new_place[seat] = false;
                    std::tie(score, pos) = parse_number(message, pos, true);
                } catch (InvalidMessageException &e) {
                    return std::make_shared<InvalidPacket>(message);
                }

                scores[seat] = score;
            }

            if (packet_type == "SCORE")
                packet = std::make_shared<Score>(scores);
            else // TOTAL
                packet = std::make_shared<Total>(scores);
        }

        if (pos < message.size())
            return std::make_shared<InvalidPacket>(message);
        return packet;
    }
}

pPacket parser::parse(std::string message) {
    if (!is_message_complete(message))
        return std::make_shared<InvalidPacket>(message);

    // Remove newline
    message = message.substr(0, message.length() - 2);

    std::vector<std::string> types = {
        "IAM", "BUSY", "DEAL", "WRONG", "TAKEN",
        "TRICK", "SCORE", "TOTAL"
    };

    size_t pos = 0;
    std::string type = "";
    for (const std::string &t: types) {
        size_t len = t.size();
        if (len <= message.size() and message.substr(0, len) == t) {
            type = t;
            pos = len;
            break;
        }
    }

    if (type.empty())
        return std::make_shared<InvalidPacket>(message);
    return parse_packet(type, message, pos);
}

game_engine::Card parser::parse_player_command(std::string command) {
    if (command.empty() or command[0] != '!')
        return game_engine::Card(
            game_engine::Card::INVALID_VALUE,
            game_engine::Card::INVALID_COLOR);

    game_engine::Card card;
    size_t pos;
    std::tie(card, pos) = parse_card(command, 1);

    if (pos != command.size())
        return game_engine::Card(
            game_engine::Card::INVALID_VALUE,
            game_engine::Card::INVALID_COLOR);

    return card;
}

std::vector<std::string> parser::split_messages(char const *s, size_t length) {
    std::vector<std::string> messages;
    std::string temp;

    for (size_t i = 0; i < length; ++i) {
        temp += s[i];
        if (i > 0 and s[i] == '\n' and s[i - 1] == '\r') {
            messages.emplace_back(temp);
            temp.clear();
        }
    }

    if (!temp.empty()) messages.emplace_back(temp);
    return messages;
}

bool parser::is_message_complete(const std::string &message) {
    size_t len = message.length();
    return (len >= 2 and message[len - 2] == '\r' and message[len - 1] == '\n');
}

std::vector<game_engine::Card> parser::parse_cards(const std::string &s) {
    return parse_cards_list(s, 0).first;
}
