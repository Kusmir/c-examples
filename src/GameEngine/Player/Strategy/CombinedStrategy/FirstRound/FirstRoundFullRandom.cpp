#include <chrono>
#include "FirstRoundFullRandom.h"

Decision FirstRoundFullRandom::redOrBlack(const GameState &game_state[[maybe_unused]]) {
    return randomBool() ? Decision::BLACK : Decision::RED;
}

Decision FirstRoundFullRandom::higherOrLower(const GameState &game_state[[maybe_unused]],
                                             const Hand &hand[[maybe_unused]]) {

    // Probability is chosen as though player didn't look at the cards.
    // Choose random value from [1, 13] and then:
    //      - [1, 6]:   lower
    //      - 7:        equal
    //      - [8, 13]:  higher

    int rd = std::uniform_int_distribution<int>(1, 13)(_rng);
    if (rd <= 6)
        return Decision::LOWER;
    else if (rd == 7)
        return Decision::EQUAL;
    else
        return Decision::HIGHER;
}

Decision FirstRoundFullRandom::insideOrOutside(const GameState &game_state[[maybe_unused]],
                                               const Hand &hand[[maybe_unused]]) {
    return randomBool() ? Decision::INSIDE : Decision::OUTSIDE;
}

FirstRoundFullRandom::FirstRoundFullRandom(long seed) : _rng(seed) {}

bool FirstRoundFullRandom::randomBool() {
    return std::uniform_int_distribution<int>(0, 1)(_rng);
}
