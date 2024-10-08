#ifndef PLAYER_H
#define PLAYER_H
#include <vector>

#include "Rules.h"
#include "Seat.h"
#include "Card.h"
#include "Trick.h"

namespace game_engine {
    struct Player {
        Seat seat;
        std::vector<Card> cards;
        std::vector<Trick> taken_tricks;
        size_t total_score;
        bool active = false;

        bool has_card(Card card);
        bool remove_card(Card card);
        bool has_color(Card::Color color);
        void take_trick(const Trick &trick);
        std::vector<Card> taken_cards();
        size_t get_score(Rules rule);
    };
}


#endif //PLAYER_H
