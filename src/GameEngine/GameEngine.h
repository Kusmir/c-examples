#ifndef TRAMWAY_GAMEENGINE_H
#define TRAMWAY_GAMEENGINE_H


#include "../GameState.h"
#include "../Visualizer/Visualizer.h"

class GameEngine {
public:
    GameEngine(long seed = std::chrono::system_clock::now().time_since_epoch().count());
    GameReport run(GameState &game_state);

    // Overtime has almost same rules as first round.
    void runFirstRound(GameState &game_state, bool overtime = false);
    void runSecondRound(GameState &game_state);
    void runThirdRound(GameState &game_state, int reshuffles = 0);
    void runOvertime(GameState &game_state);

    void set_visualize(bool visualize);
private:
    std::mt19937 _rng;
    Visualizer _visualizer;

    void endSecondRound(GameState &game_state);
    void endOvertime(GameState &game_state);
};


#endif //TRAMWAY_GAMEENGINE_H
