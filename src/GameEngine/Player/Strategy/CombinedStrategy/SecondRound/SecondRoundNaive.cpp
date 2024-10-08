#include "SecondRoundNaive.h"

Decision SecondRoundNaive::shouldPlaceCard(const GameState &game_state[[maybe_unused]],
                                           const Hand &hand[[maybe_unused]]) {
    return Decision::PLACE;
}
