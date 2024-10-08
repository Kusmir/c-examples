#ifndef TRAMWAY_ISTRATEGY_H
#define TRAMWAY_ISTRATEGY_H


#include "../../Decision.h"
#include "../../../GameState.h"
#include "../../Hand.h"

class IStrategy {
public:
    virtual Decision makeDecision(const GameState &game_state, const Hand &hand) = 0;
};

#endif //TRAMWAY_ISTRATEGY_H
