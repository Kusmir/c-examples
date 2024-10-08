#ifndef TRAMWAY_INTERACTIVESTRATEGY_H
#define TRAMWAY_INTERACTIVESTRATEGY_H


#include "IStrategy.h"
#include "../../Hand.h"
#include "../../../GameState.h"

class InteractiveStrategy : public IStrategy {
public:
    InteractiveStrategy() = default;
    Decision makeDecision(const GameState &game_state, const Hand &hand) override;
};


#endif //TRAMWAY_INTERACTIVESTRATEGY_H
