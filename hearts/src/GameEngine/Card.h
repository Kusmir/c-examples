#ifndef CARD_H
#define CARD_H

#include <optional>
#include <ostream>
#include <string>

namespace game_engine {
class Card {
public:
    enum Value {
        TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
        TEN, JACK, QUEEN, KING, ACE, INVALID_VALUE
    };

    enum Color {
        HEARTS, SPADES, CLUBS, DIAMONDS, INVALID_COLOR
    };

    Card(const Value value = INVALID_VALUE,
         const Color color = INVALID_COLOR)
        : value_(value), color_(color) {
    }

    [[nodiscard]] Value value() const { return value_; }
    [[nodiscard]] Color color() const { return color_; }

    [[nodiscard]] std::string to_string(bool pretty = false) const;

    static std::string value_to_string(Value value);

    static std::string color_to_string(Color color, bool pretty);

    static std::optional<Value> string_to_value(const std::string &str);

    friend inline bool operator==(const Card &lhs, const Card &rhs) {
        return lhs.value_ == rhs.value_
               and lhs.color_ == rhs.color_;
    }

    friend inline bool operator!=(const Card &lhs, const Card &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const Card &lhs, const Card &rhs) {
        if (lhs.value_ < rhs.value_)
            return true;
        if (rhs.value_ < lhs.value_)
            return false;
        return lhs.color_ < rhs.color_;
    }

    friend bool operator<=(const Card &lhs, const Card &rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const Card &lhs, const Card &rhs) {
        return rhs < lhs;
    }

    friend bool operator>=(const Card &lhs, const Card &rhs) {
        return !(lhs < rhs);
    }

    static std::optional<Color> char_to_color(char c);

    friend std::ostream &operator<<(std::ostream &os, const Card &card) {
        return os << card.to_string();
    }

private:
    Value value_;
    Color color_;
};
}


#endif //CARD_H
