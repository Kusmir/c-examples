#ifndef TRAMWAY_THIRDROUNDSTRATEGY_H
#define TRAMWAY_THIRDROUNDSTRATEGY_H


#include "../../IStrategy.h"

class ThirdRoundStrategy {
public:
    virtual Decision makeDecision(const GameState &game_state, const Hand &hand) = 0;
};


#endif //TRAMWAY_THIRDROUNDSTRATEGY_H
