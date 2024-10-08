#ifndef TRAMWAY_THIRDROUNDNAIVE_H
#define TRAMWAY_THIRDROUNDNAIVE_H


#include "ThirdRoundStrategy.h"

// Chooses higher or lower based on last card value.

class ThirdRoundNaive : public ThirdRoundStrategy {
public:
    ThirdRoundNaive() = default;
    Decision makeDecision(const GameState &game_state, const Hand &hand) override;
};


#endif //TRAMWAY_THIRDROUNDNAIVE_H
