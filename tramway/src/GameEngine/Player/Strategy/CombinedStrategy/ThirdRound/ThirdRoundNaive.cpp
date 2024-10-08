#include "ThirdRoundNaive.h"

Decision ThirdRoundNaive::makeDecision(const GameState &game_state, const Hand &hand[[maybe_unused]]) {
    Card::Value last_card_value = game_state.last_revealed_card().value();

    if (last_card_value <= Card::Value::SEVEN)
        return Decision::HIGHER;
    else
        return Decision::LOWER;
}
