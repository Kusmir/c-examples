#ifndef TRAMWAY_COMBINEDSTRATEGY_H
#define TRAMWAY_COMBINEDSTRATEGY_H

#include "../IStrategy.h"
#include "FirstRound/FirstRoundStrategy.h"
#include "ThirdRound/ThirdRoundStrategy.h"
#include "SecondRound/SecondRoundStrategy.h"

class CombinedStrategy : public IStrategy {
public:

    Decision makeDecision(const GameState &game_state, const Hand &hand) override;

    CombinedStrategy(const std::shared_ptr<FirstRoundStrategy> &firstRoundStrategy,
                     const std::shared_ptr<SecondRoundStrategy> &secondRoundStrategy,
                     const std::shared_ptr<ThirdRoundStrategy> &thirdRoundStrategy);

    static std::shared_ptr<CombinedStrategy> p_NaiveStrategy();

private:
    std::shared_ptr<FirstRoundStrategy> _first_round_strategy;
    std::shared_ptr<SecondRoundStrategy> _second_round_strategy;
    std::shared_ptr<ThirdRoundStrategy> _third_round_strategy;

};


#endif //TRAMWAY_COMBINEDSTRATEGY_H
