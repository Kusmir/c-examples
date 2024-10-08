#ifndef TRAMWAY_DECK_H
#define TRAMWAY_DECK_H

#include <vector>
#include <chrono>
#include <random>

#include "Card.h"


class Deck {
public:
    Deck(int number_of_decks = 1, long seed = std::chrono::system_clock::now().time_since_epoch().count());

    void shuffle();
    Card dealCard();
    size_t size() const;
    size_t default_size() const;
    bool empty() const;
    void reset();

    void set_seed(long seed);

private:
    std::mt19937 _rng;
    std::vector<Card> _content;

    int _number_of_decks;
    void initializeDeckContent();
};


#endif //TRAMWAY_DECK_H
