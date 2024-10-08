#ifndef DEAL_H
#define DEAL_H

#include <vector>
#include <string>

#include "Packet.h"
#include "../../GameEngine/Seat.h"
#include "../../GameEngine/Card.h"

class Deal : public Packet{
public:
    [[nodiscard]] std::string get_message() const override {
        std::string ret = "DEAL" + std::to_string(ruleset_);
        ret += seat_to_char(first_player_position_);
        for (const game_engine::Card &card : cards_) {
            ret += card.to_string();
        }
        return ret + "\r\n";
    }

    [[nodiscard]] size_t ruleset() const {
        return ruleset_;
    }

    [[nodiscard]] char first_player_position() const {
        return first_player_position_;
    }

    Deal(size_t ruleset, Seat first_player_position,
        const std::vector<game_engine::Card> &cards)
        : ruleset_(ruleset),
          first_player_position_(first_player_position),
          cards_(cards) {
        type_ = DEAL;
    }

    [[nodiscard]] std::vector<game_engine::Card> cards() const {
        return cards_;
    }

private:
    size_t ruleset_;
    Seat first_player_position_;
    std::vector<game_engine::Card> cards_;

};



#endif //DEAL_H
