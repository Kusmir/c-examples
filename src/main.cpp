#include <iostream>

#include "GameEngine/Player/Player.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/Player/Strategy/InteractiveStrategy.h"

int main() {
    auto bot1 = std::make_shared<Player>("Player1", CombinedStrategy::p_NaiveStrategy());
    auto bot2 = std::make_shared<Player>("Player2", CombinedStrategy::p_NaiveStrategy());
    auto player1 = std::make_shared<Player>("Real player", std::make_shared<InteractiveStrategy>());
    GameState game_state({bot1, bot2, player1});

    GameEngine game_engine;
    game_engine.set_visualize(true);

    game_engine.run(game_state);
}
