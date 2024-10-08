#include "Deck.h"

#include <random>
#include <chrono>
#include <algorithm>

#include "../Exceptions/EmptyDeckException.h"

void Deck::initializeDeckContent() {
    _content = std::vector<Card>();
    _content.reserve(_number_of_decks * 52);

    for (int i = 0; i < _number_of_decks; ++i) {
        for (Card::Suit suit: {Card::Suit::CLUBS, Card::Suit::DIAMONDS,
                               Card::Suit::HEARTS, Card::Suit::SPADES}) {
            for (int value_int = Card::Value::TWO; value_int <= Card::Value::ACE; ++value_int) {
                _content.emplace_back(Card::Value(value_int), suit);
            }
        }
    }
}

Deck::Deck(int number_of_decks, long seed) : _rng(seed), _number_of_decks(number_of_decks) {
    initializeDeckContent();
}

void Deck::shuffle() {
    std::shuffle(_content.begin(), _content.end(), _rng);
}

Card Deck::dealCard() {
    if (_content.empty())
        throw EmptyDeckException();

    Card card = _content.back();
    _content.pop_back();
    return card;
}

size_t Deck::size() const {
    return _content.size();
}

bool Deck::empty() const {
    return Deck::size() == 0;
}

void Deck::set_seed(long seed) {
    _rng = std::mt19937(seed);
}

void Deck::reset() {
    initializeDeckContent();
}

size_t Deck::default_size() const {
    return _number_of_decks * 52;
}

