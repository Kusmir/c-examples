#include "CombinedStrategy.h"
#include "FirstRound/FirstRoundNaive.h"
#include "SecondRound/SecondRoundNaive.h"
#include "ThirdRound/ThirdRoundNaive.h"
#include "../../../../Exceptions/InvalidGameStateException.h"


Decision CombinedStrategy::makeDecision(const GameState &game_state, const Hand &hand) {
    switch (game_state.round_id()) {
        case 1:
            return _first_round_strategy->makeDecision(game_state, hand);
        case 2:
            return _second_round_strategy->makeDecision(game_state, hand);
        case 3:
            return _third_round_strategy->makeDecision(game_state, hand);
        default:
            throw InvalidGameStateException("Invalid round number" + std::to_string(game_state.round_id()));
    }
}

CombinedStrategy::CombinedStrategy(const std::shared_ptr<FirstRoundStrategy> &firstRoundStrategy,
                                   const std::shared_ptr<SecondRoundStrategy> &secondRoundStrategy,
                                   const std::shared_ptr<ThirdRoundStrategy> &thirdRoundStrategy)
        : _first_round_strategy(firstRoundStrategy),
          _second_round_strategy(secondRoundStrategy),
          _third_round_strategy(thirdRoundStrategy) {}

std::shared_ptr<CombinedStrategy> CombinedStrategy::p_NaiveStrategy() {
    return std::make_shared<CombinedStrategy>(
            std::make_shared<FirstRoundNaive>(),
            std::make_shared<SecondRoundNaive>(),
            std::make_shared<ThirdRoundNaive>()
    );
}
