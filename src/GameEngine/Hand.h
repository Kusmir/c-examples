#ifndef TRAMWAY_HAND_H
#define TRAMWAY_HAND_H


#include <vector>
#include "Card.h"

class Hand {
public:
    explicit Hand(std::vector<Card> cards = {});

    bool containsValue(Card::Value value) const;
    bool remove(Card card);
    void add(Card card);
    void clear();
    size_t size() const;
    bool empty() const;

    const Card& operator [] (size_t index) const { return _content[index]; };
    friend std::ostream& operator << (std::ostream &os, const Hand &hand);

    const std::vector<Card>& content() const { return _content; }

private:
    std::vector<Card> _content;
};


#endif //TRAMWAY_HAND_H
