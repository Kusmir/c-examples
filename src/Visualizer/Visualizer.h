#ifndef TRAMWAY_VISUALIZER_H
#define TRAMWAY_VISUALIZER_H


#include "../GameState.h"
#include "../Reporter/GameReport.h"

class Visualizer {
public:
    Visualizer(bool active = false);
    void display(const GameState &game_state, Decision decision = Decision::NONE);

    void displayDecision(const GameState &game_state, Decision player_decision, Decision correct_decision);
    void displayPunishment(const GameState &game_state, std::shared_ptr<Player> p_player, int strikes);
    void displayEndScreen(const GameReport &game_report);

    void set_active(bool active);

private:
    bool _active;
    static void displayFirstRound(const GameState &game_state, Decision decision = Decision::NONE);
    static void displaySecondRound(const GameState &game_state, Decision decision = Decision::NONE);
    static void displayThirdRound(const GameState &game_state, Decision decision = Decision::NONE);

    static void printPlayer(const GameState &game_state, const std::shared_ptr<Player> &p_player);
    static void printBold(const std::string &string);
    static void printDecision(Decision decision);
    static void waitForInput();
};


#endif //TRAMWAY_VISUALIZER_H
