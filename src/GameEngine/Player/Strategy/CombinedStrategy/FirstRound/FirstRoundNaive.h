#ifndef TRAMWAY_FIRSTROUNDNAIVE_H
#define TRAMWAY_FIRSTROUNDNAIVE_H


#include "FirstRoundStrategy.h"

// Makes decision using heuristics without analyzing entire game state.

class FirstRoundNaive : public FirstRoundStrategy {
protected:
    // Always choose red.
    Decision redOrBlack(const GameState &game_state) override;

    // Decide basing on card value.
    // <= 7: higher
    //  > 7: lower
    Decision higherOrLower(const GameState &game_state, const Hand &hand) override;

    // Decide basing only on range length.
    Decision insideOrOutside(const GameState &game_state, const Hand &hand) override;
};


#endif //TRAMWAY_FIRSTROUNDNAIVE_H
