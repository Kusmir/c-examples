#include "Card.h"

#include <regex>
#include <string>
#include <unordered_map>

namespace {
    const std::string RED = "\033[31m";
    const std::string BLACK = "\033[30m";
    const std::string BOLD = "\033[1m";
    const std::string RESET = "\033[0m";
}


std::string game_engine::Card::value_to_string(Value value) {
    switch (value) {
        case TWO: return "2";
        case THREE: return "3";
        case FOUR: return "4";
        case FIVE: return "5";
        case SIX: return "6";
        case SEVEN: return "7";
        case EIGHT: return "8";
        case NINE: return "9";
        case TEN: return "10";
        case JACK: return "J";
        case QUEEN: return "Q";
        case KING: return "K";
        case ACE: return "A";
        default: return "?";
    }
}

std::string game_engine::Card::color_to_string(Color color, bool pretty = false) {
    switch (color) {
        case CLUBS:
            if (pretty) return "\u2663";
            return "C";

        case DIAMONDS:
            if (pretty) return "\u2666";
            return "D";

        case HEARTS:
            if (pretty) return "\u2665";
            return "H";

        case SPADES:
            if (pretty) return "\u2660";
            return "S";

        default:
            return "?";
    }
}

std::optional<game_engine::Card::Value> game_engine::Card::string_to_value(const std::string &str) {
    static const std::unordered_map<std::string, Value>
            string_to_value_map = {
                {"2", TWO},
                {"3", THREE},
                {"4", FOUR},
                {"5", FIVE},
                {"6", SIX},
                {"7", SEVEN},
                {"8", EIGHT},
                {"9", NINE},
                {"10",TEN},
                {"J", JACK},
                {"Q", QUEEN},
                {"K", KING},
                {"A", ACE}
            };

    auto it = string_to_value_map.find(str);
    if (it != string_to_value_map.end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}

std::optional<game_engine::Card::Color> game_engine::Card::char_to_color(const char c) {
    static const std::unordered_map<char, Color>
            char_to_color_map = {
                {'C', CLUBS},
                {'S', SPADES},
                {'D', DIAMONDS},
                {'H', HEARTS}
            };

    auto it = char_to_color_map.find(c);
    if (it != char_to_color_map.end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}

std::string game_engine::Card::to_string(bool pretty) const {
    std::string value_string = value_to_string(value_);

    std::string color_string = color_to_string(color_);

    if (pretty) {
        bool is_red = (color_ == DIAMONDS or color_ == HEARTS);
        std::string color_modifier = (is_red ? RED : BLACK);
        return color_modifier + BOLD + value_string + color_string + RESET;
    } else {
        return value_string + color_string;
    }
}

