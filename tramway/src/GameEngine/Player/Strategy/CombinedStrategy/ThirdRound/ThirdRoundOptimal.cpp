#include "ThirdRoundOptimal.h"

Decision ThirdRoundOptimal::makeDecision(const GameState &game_state, const Hand &hand[[maybe_unused]]) {
    Card::Value current_value = game_state.last_revealed_card().value();
    int deck_size = (int)game_state.default_deck_size();
    int default_value_count = deck_size / 13;

    int lower = current_value * default_value_count;
    int equal = default_value_count;
    int higher = deck_size - lower - equal;

    for (const std::vector<Card> &pile : game_state.table()) {
        for (const Card &card : pile) {
            if (card.value() < current_value)
                --lower;
            else if (card.value() == current_value)
                --equal;
            else if (card.value() > current_value)
                --higher;
        }
    }

    int best_count = std::max({lower, equal, higher});
    if (lower == best_count)
        return Decision::LOWER;
    else if (higher == best_count)
        return Decision::HIGHER;
    else
        return Decision::EQUAL;
}
