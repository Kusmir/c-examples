#include <iostream>
#include "Visualizer.h"
#include "../GameEngine/Player/Player.h"
#include "../GameEngine/Card.h"
#include "../Exceptions/InvalidGameStateException.h"

void Visualizer::display(const GameState &game_state, Decision decision) {
    if (!_active) return;

    std::system("clear");
    switch (game_state.round_id()) {
        case 1:
            std::cout << "=======<  Round 1  >=======\n\n";
            displayFirstRound(game_state, decision);
            break;
        case 2:
            std::cout << "=======<  Round 2  >=======\n\n";
            displaySecondRound(game_state, decision);
            break;
        case 3:
            std::cout << "=======<  Round 3  >=======\n\n";
            displayThirdRound(game_state, decision);
            break;
        default:
            throw InvalidGameStateException("Invalid round number.");
    }
}

void Visualizer::displayDecision(const GameState &game_state, Decision player_decision, Decision correct_decision) {
    if (!_active) return;

    display(game_state, player_decision);
    if (player_decision == correct_decision)
        std::cout << "Correct\n";
    else
        std::cout << "Wrong\n";
    waitForInput();
}



void Visualizer::displayFirstRound(const GameState &game_state, Decision decision) {
    for (const std::shared_ptr<Player> &p_player : game_state.players()) {
        printPlayer(game_state, p_player);
        if (p_player == game_state.active_player())
            printDecision(decision);
        std::cout << '\n';
    }
}

void Visualizer::displaySecondRound(const GameState &game_state, Decision decision) {
    auto table = game_state.table();
    std::string empty_space = "   ";

    int max_level = game_state.game_rules().max_second_round_level;
    size_t pile_index = table.size() - 1;
    for (int level = 1; level <= max_level; ++level) {
        int blanks = max_level - level;

        for (int row = 0; row < 2; ++row) {
            for (int i = 0; i < blanks; ++i)
                std::cout << empty_space;
            for (int i = 0; i < level; ++i) {
                const std::vector<Card> &pile = table[pile_index--];
                if ((int)pile.size() > row)
                    std::cout << pile[row];
                else
                    std::cout << empty_space;

                std::cout << empty_space;
            }
            if (row == 0)
                pile_index += level;
            std::cout << '\n';
        }
    }

    for (const std::shared_ptr<Player> &p_player : game_state.players()) {
        printPlayer(game_state, p_player);
        if (p_player == game_state.active_player())
            printDecision(decision);
        std::cout << '\n';
    }

}

void Visualizer::displayThirdRound(const GameState &game_state, Decision decision) {
    auto table = game_state.table();
    std::string empty_space = " ";

    bool run = true;
    for (int row = 0; run; ++row) {
        run = false;
        for (const std::vector<Card> &pile : table) {
            if ((int)pile.size() > row ) {
                std::cout << pile[row] << empty_space;
                run = true;
            } else {
                std::cout << empty_space;
            }
        }
        std::cout << '\n';
    }

    printPlayer(game_state, game_state.active_player());
    std::cout << ' ';
    printDecision(decision);
    std::cout << '\n';
}

void Visualizer::printBold(const std::string &string) {
    std::cout << "\033[1m" << string << "\033[0m";
}

void Visualizer::printPlayer(const GameState &game_state, const std::shared_ptr<Player> &p_player) {
    if (game_state.active_player() == p_player)
        printBold(p_player->name());
    else std::cout << p_player->name();

    std::cout << ' ';
    if (!p_player->hand().empty())
        std::cout << p_player->hand() << ' ';

    std::cout << "(strikes: " << p_player->strike_counter() << ")";
}

void Visualizer::printDecision(Decision decision) {
    switch (decision) {
        case Decision::LOWER:
            std::cout << "<Lower>";
            break;
        case Decision::EQUAL:
            std::cout << "<Equal>";
            break;
        case Decision::HIGHER:
            std::cout << "<Higher>";
            break;
        case Decision::RED:
            std::cout << "<Red>";
            break;
        case Decision::BLACK:
            std::cout << "<Black>";
            break;
        case Decision::INSIDE:
            std::cout << "<Inside>";
            break;
        case Decision::OUTSIDE:
            std::cout << "<Outside>";
            break;
        case Decision::PLACE:
            std::cout << "<Place>";
            break;
        case Decision::HOLD:
            std::cout << "<Hold>";
            break;
        case Decision::NONE:
            break;
    }
}

void Visualizer::displayPunishment(const GameState &game_state, std::shared_ptr<Player> p_player, int strikes) {
    if (!_active) return;

    display(game_state, Decision::PLACE);
    std::cout << p_player->name() << " gets " << strikes << " strikes.\n";
    waitForInput();
}

Visualizer::Visualizer(bool active) : _active(active) {}

void Visualizer::set_active(bool active) {
    _active = active;
}

void Visualizer::waitForInput() {
    std::cin.get();
}

void Visualizer::displayEndScreen(const GameReport &game_report) {
    if (!_active) return;

    std::cout << "THE END\n\nResults:\n";
    for (GameReport::Entry entry : game_report.results)
        std::cout << entry.player_name << ": " << entry.total_score << '\n';
}

