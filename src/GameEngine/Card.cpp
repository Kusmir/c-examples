#include "Card.h"

Card::Card(Value value, Suit suit, bool flipped)
        : _suit(suit), _value(value), _flipped(flipped) {}

bool Card::operator==(const Card &other) const {
    return (_value == other._value and _suit == other._suit);
}

std::string Card::toString() {
    std::string ret = " ";
    if (value() == TEN)
        ret = "";

    if (value() <= TEN) {
        // Add 2, because lowest indexed (index 0) value is 2.
        ret += std::to_string(_value + 2);
    }
    else if (value() == JACK) ret += 'J';
    else if (value() == QUEEN) ret += 'Q';
    else if (value() == KING) ret += 'K';
    else if (value() == ACE) ret += 'A';
    else if (value() == NONE) ret += '#';

    switch (suit()) {
        case CLUBS:
            ret += "♣";
            break;
        case DIAMONDS:
            ret += "♦";
            break;
        case HEARTS:
            ret += "♥";
            break;
        case SPADES:
            ret += "♠";
            break;
        case CARD_BACK:
            ret += "#";
            break;
    }

    return ret;
}

std::ostream& operator << (std::ostream &os, Card card) {
    std::string color_prompt_begin = "\033[1m";
    std::string color_prompt_end = "\033[0m";
    if (card.suit() == Card::Suit::DIAMONDS or card.suit() == Card::Suit::HEARTS) {
        color_prompt_begin = "\033[1;31m";
        color_prompt_end = "\033[0m";
    }

    return os << color_prompt_begin << card.toString() << color_prompt_end;
}

Decision Card::color() const {
    if (_suit == Card::Suit::SPADES or _suit == Card::Suit::CLUBS)
        return Decision::BLACK;
    else
        return Decision::RED;
}

Card::Suit Card::suit() const {
    if (_flipped)
        return Card::Suit::CARD_BACK;
    return _suit;
}

Card::Value Card::value() const {
    if (_flipped)
        return Card::Value::NONE;
    return _value;
}

Card& Card::faceUp() {
    _flipped = false;
    return *this;
}

Card& Card::faceDown() {
    _flipped = true;
    return *this;
}
