#include "FirstRoundNaive.h"

Decision FirstRoundNaive::redOrBlack(const GameState &game_state[[maybe_unused]]) {
    return Decision::RED;
}

Decision FirstRoundNaive::higherOrLower(const GameState &game_state[[maybe_unused]], const Hand &hand) {
    Card::Value value= hand[0].value();
    if (value <= Card::Value::SEVEN)
        return Decision::HIGHER;
    else
        return Decision::LOWER;
}

Decision FirstRoundNaive::insideOrOutside(const GameState &game_state[[maybe_unused]], const Hand &hand) {
    Card::Value first_value = hand[0].value();
    Card::Value second_value = hand[1].value();
    if (first_value > second_value)
        std::swap(first_value, second_value);

    int segment_length = second_value - first_value + 1;
    int half = (Card::Value::ACE - Card::Value::TWO + 1) / 2;

    return segment_length >= half ? Decision::INSIDE : Decision::OUTSIDE;
}
