#ifndef TRAMWAY_CARD_H
#define TRAMWAY_CARD_H


#include <string>
#include "Decision.h"

class Card {
public:
    enum Suit {
        CLUBS, DIAMONDS, HEARTS, SPADES, CARD_BACK
    };

    // Value is used for indexing and cannot be changed.
    enum Value {
        TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
        JACK, QUEEN, KING, ACE, NONE
    };

    bool operator == (const Card &other) const;
    friend std::ostream& operator << (std::ostream &os, Card card);

    std::string toString();

    Card(Value value, Suit suit, bool flipped = false);
    Suit suit() const;
    Value value() const;
    Decision color() const; // TODO: this method is suspicious.
    Card& faceUp();
    Card& faceDown();

private:
    Suit _suit;
    Value _value;
    bool _flipped;
};


#endif //TRAMWAY_CARD_H
