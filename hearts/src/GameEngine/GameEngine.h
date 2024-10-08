#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <array>
#include <fstream>

#include "Card.h"
#include "Player.h"
#include "Rules.h"
#include "Seat.h"
#include "Trick.h"

namespace game_engine {
    class GameEngine {
    public:
        static constexpr size_t MAX_TRICKS = 13;
        static constexpr size_t PLAYERS = 4;

        GameEngine(const std::string &description_path);
        GameEngine() {};
        Seat determine_winner();
        std::array<std::vector<Card>, PLAYERS> start_new_deal();
        std::array<size_t, 4> end_deal();
        std::array<size_t, 4> get_total_scores();
        bool place_card(Seat seat, Card card);
        std::array<bool, 4> taken_seats() const;
        bool is_seat_taken(Seat seat) const;
        bool add_player(Seat seat);
        bool remove_player(Seat seat);
        size_t active_players() const;
        bool paused() const;
        bool finished() { return finished_; }

        size_t current_trick_id() const { return current_trick_.id; }
        Seat active_player_seat() const { return active_player_seat_; }
        Trick current_trick() const { return current_trick_; }
        size_t deal_id() const { return deal_id_; }
        Seat first_seat() const { return first_seat_; }
        Rules rules() const { return rules_; }

    private:
        std::ifstream description_{};
        Seat active_player_seat_ = N;
        Seat first_seat_ = N;

        std::array<Player, 4> players_{};
        Trick current_trick_{};
        Rules rules_ = ALL;
        size_t deal_id_;
        bool finished_;
    };
}



#endif //GAMEENGINE_H
