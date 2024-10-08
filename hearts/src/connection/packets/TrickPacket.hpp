#ifndef TRICKPACKET_H
#define TRICKPACKET_H


#include <vector>

#include "Packet.h"

class TrickPacket : public Packet {
public:
    [[nodiscard]] size_t trick_id() const {
        return trick_id_;
    }

    [[nodiscard]] std::vector<game_engine::Card> cards() const {
        return cards_;
    }

    TrickPacket(size_t trick_id, const std::vector<game_engine::Card> &cards)
        : trick_id_(trick_id),
          cards_(cards) {
        type_ = TRICK;
    }

    TrickPacket(size_t trick_id, game_engine::Card card)
        : trick_id_(trick_id),
          cards_({card}) {
        type_ = TRICK;
    }

    [[nodiscard]] std::string get_message() const override {
        std::string ret = "TRICK";
        ret += std::to_string(trick_id_);
        for (const game_engine::Card &card : cards_)
            ret += card.to_string();
        return ret + "\r\n";
    }

private:
    size_t trick_id_;
    std::vector<game_engine::Card> cards_;
};



#endif //TRICKPACKET_H
