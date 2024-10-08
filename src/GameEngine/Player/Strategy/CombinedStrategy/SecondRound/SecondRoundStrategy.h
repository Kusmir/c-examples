#ifndef TRAMWAY_SECONDROUNDSTRATEGY_H
#define TRAMWAY_SECONDROUNDSTRATEGY_H


#include "../../IStrategy.h"
#include "../../../../../GameState.h"

class SecondRoundStrategy {
public:
    Decision makeDecision(const GameState &game_state[[maybe_unused]], const Hand &hand[[maybe_unused]]);
protected:
    // Makes decision with assumption that player has a card of needed value.
    virtual Decision shouldPlaceCard(const GameState &game_state, const Hand &hand) = 0;
};


#endif //TRAMWAY_SECONDROUNDSTRATEGY_H

