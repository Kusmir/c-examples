#include "Hand.h"

#include <utility>
#include <iostream>

Hand::Hand(std::vector<Card> cards) : _content(std::move(cards)) {}

bool Hand::containsValue(Card::Value value) const {
    for (Card card : _content) {
        if (card.value() == value)
            return true;
    }
    return false;
}

bool Hand::remove(Card card) {
    for (auto it = _content.begin(); it != _content.end(); ++it) {
        if (*it == card) {
            _content.erase(it);
            return true;
        }
    }
    return false;
}

void Hand::add(Card card) {
    _content.push_back(card);
}

size_t Hand::size() const {
    return _content.size();
}

void Hand::clear() {
    _content.clear();
}

std::ostream& operator << (std::ostream &os, const Hand &hand) {
    os << "[";
    for (size_t i = 0; i < 3; ++i) {
        if (i < hand.size())
            os << hand[i] << ' ';
    }
    return os << "]";
}

bool Hand::empty() const {
    return size() == 0;
}
