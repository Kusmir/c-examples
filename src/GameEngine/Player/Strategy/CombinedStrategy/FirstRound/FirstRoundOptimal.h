#ifndef TRAMWAY_FIRSTROUNDOPTIMAL_H
#define TRAMWAY_FIRSTROUNDOPTIMAL_H


#include "../../../../Decision.h"
#include "../../../../../GameState.h"
#include "FirstRoundStrategy.h"

class FirstRoundOptimal : public FirstRoundStrategy {
public:
    FirstRoundOptimal() = default;

private:
    Decision redOrBlack(const GameState &game_state) override;

    Decision
    higherOrLower(const GameState &game_state, const Hand &hand) override;

    Decision insideOrOutside(const GameState &game_state, const Hand &hand) override;

};


#endif //TRAMWAY_FIRSTROUNDOPTIMAL_H
