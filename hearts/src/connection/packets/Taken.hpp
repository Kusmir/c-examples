#ifndef TAKEN_H
#define TAKEN_H

#include "Packet.h"

class Taken : public Packet {
private:
    size_t trick_id_;
    std::vector<game_engine::Card> cards_;
    Seat active_seat_;

public:
    [[nodiscard]] std::string get_message() const override {
        std::string ret = "TAKEN";
        ret += std::to_string(trick_id_);
        for (const game_engine::Card &card : cards_)
            ret += card.to_string();
        ret += seat_to_char(active_seat_);
        return ret + "\r\n";
    }

    Taken(size_t turn_id, const std::vector<game_engine::Card> &cards,
        const Seat &active_seat)
        : trick_id_(turn_id),
          cards_(cards),
          active_seat_(active_seat) {
        type_ = TAKEN;
    }

    [[nodiscard]] size_t trick_id() const {
        return trick_id_;
    }

    [[nodiscard]] std::vector<game_engine::Card> cards() const {
        return cards_;
    }

    [[nodiscard]] Seat active_seat() const {
        return active_seat_;
    }
};



#endif //TAKEN_H
