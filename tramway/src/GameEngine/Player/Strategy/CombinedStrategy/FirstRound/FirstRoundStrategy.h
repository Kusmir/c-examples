#ifndef TRAMWAY_FIRSTROUNDSTRATEGY_H
#define TRAMWAY_FIRSTROUNDSTRATEGY_H


#include "../../IStrategy.h"
#include "../../../../Card.h"

class FirstRoundStrategy {
public:
    Decision makeDecision(const GameState &game_state[[maybe_unused]], const Hand &hand[[maybe_unused]]);

protected:
    virtual Decision redOrBlack(const GameState &game_state) = 0;

    virtual Decision
    higherOrLower(const GameState &game_state, const Hand &hand[[maybe_unused]]) = 0;

    virtual Decision
    insideOrOutside(const GameState &game_state, const Hand &hand [[maybe_unused]]) = 0;
};


#endif //TRAMWAY_FIRSTROUNDSTRATEGY_H
