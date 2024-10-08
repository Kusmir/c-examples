#include "FirstRoundStrategy.h"
#include "../../../../../Exceptions/InvalidGameStateException.h"

Decision FirstRoundStrategy::makeDecision(const GameState &game_state, const Hand &hand) {
    switch (hand.size()) {
        case 0:
            return redOrBlack(game_state);
        case 1:
            return higherOrLower(game_state, hand);
        case 2:
            return insideOrOutside(game_state, hand);
    }

    throw InvalidGameStateException("Player has more than three cards in round 1.");
}
