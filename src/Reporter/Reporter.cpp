#include "Reporter.h"
#include "../GameEngine/Player/Player.h"

GameReport Reporter::prepareGameReport(const GameState &game_state) {
    GameReport game_report;
    for (const std::shared_ptr<Player> &p_player : game_state.starting_players())
       game_report.results.emplace_back(p_player->name(),
                                        std::array<int, 3>(p_player->round_scores()),
                                        p_player->strike_counter());
    return game_report;
}
