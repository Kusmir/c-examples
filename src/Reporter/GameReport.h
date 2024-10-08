#ifndef TRAMWAY_GAMEREPORT_H
#define TRAMWAY_GAMEREPORT_H


#include <utility>
#include <vector>
#include <string>
#include <array>
#include <sstream>

struct GameReport {
public:
    struct Entry {
        Entry(std::string _player_name, std::array<int, 3> _round_scores, int _total_score);

        std::string player_name;
        std::array<int, 3> round_scores;
        int total_score;
    };

    std::vector<Entry> results;

    std::string formatCSV() const;
};


#endif //TRAMWAY_GAMEREPORT_H
