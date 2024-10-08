#ifndef TRAMWAY_FIRSTROUNDFULLRANDOM_H
#define TRAMWAY_FIRSTROUNDFULLRANDOM_H


#include <random>
#include "FirstRoundStrategy.h"

class FirstRoundFullRandom : public FirstRoundStrategy {
public:
    FirstRoundFullRandom(
            long seed = std::chrono::system_clock::now().time_since_epoch().count());

private:
    std::mt19937 _rng;
    bool randomBool();

    Decision redOrBlack(const GameState &game_state) override;

    Decision
    higherOrLower(const GameState &game_state, const Hand &hand) override;

    Decision insideOrOutside(const GameState &game_state, const Hand &hand) override;
};


#endif //TRAMWAY_FIRSTROUNDFULLRANDOM_H
