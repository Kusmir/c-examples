#ifndef TRAMWAY_SECONDROUNDNAIVE_H
#define TRAMWAY_SECONDROUNDNAIVE_H


#include "SecondRoundStrategy.h"
#include "../../../../Hand.h"

// Always puts a card if possible.

class SecondRoundNaive : public SecondRoundStrategy {
public:
    SecondRoundNaive() = default;

private:
    Decision shouldPlaceCard(const GameState &game_state, const Hand &hand) override;
};


#endif //TRAMWAY_SECONDROUNDNAIVE_H
