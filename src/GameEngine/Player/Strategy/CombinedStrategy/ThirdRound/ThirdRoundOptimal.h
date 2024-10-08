#ifndef TRAMWAY_THIRDROUNDOPTIMAL_H
#define TRAMWAY_THIRDROUNDOPTIMAL_H


#include "GameEngine/Decision.h"
#include "GameState.h"
#include "GameEngine/Hand.h"

class ThirdRoundOptimal : public ThridRoundStrategy {
public:
    ThirdRoundOptimal() = default;
    Decision makeDecision(const GameState &game_state, const Hand &hand) override;
};


#endif //TRAMWAY_THIRDROUNDOPTIMAL_H
