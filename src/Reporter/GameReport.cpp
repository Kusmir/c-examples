#include "GameReport.h"

#include <utility>

std::string GameReport::formatCSV() const {
    std::stringstream output;
    for (const Entry &entry : results) {
        output << entry.player_name << "; ";
        for (int score : entry.round_scores)
            output << score << "; ";
        output << entry.total_score << ";\n";
    }

    return output.str();
}

GameReport::Entry::Entry(std::string _player_name, std::array<int, 3> _round_scores, int _total_score)
    : player_name(std::move(_player_name)), round_scores(_round_scores), total_score(_total_score) {}
