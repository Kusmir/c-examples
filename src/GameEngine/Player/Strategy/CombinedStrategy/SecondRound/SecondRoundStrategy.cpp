#include "SecondRoundStrategy.h"

#include "../../../Player.h"

Decision SecondRoundStrategy::makeDecision(const GameState &game_state, const Hand &hand) {
    Card::Value current_card_value = game_state.last_revealed_card().value();
    if (!game_state.active_player()->hasCardOfValue(current_card_value))
        return Decision::HOLD;
    return shouldPlaceCard(game_state, hand);
}
