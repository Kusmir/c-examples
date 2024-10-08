#ifndef SCORE_H
#define SCORE_H
#include "Packet.h"
#include <array>

#include "../../common.h"

class Score : public Packet {
public:
    [[nodiscard]] std::string get_message() const override {
        std::string ret = "SCORE";
        for (int seat = 0; seat < (int)scores_.size(); ++seat) {
            ret += std::string(1, seat_to_char(seat));
            ret += std::to_string(scores_[seat]);
        }

        return ret + "\r\n";
    };

    explicit Score(const std::array<size_t, 4> &scores)
        : scores_(scores) {
        type_ = SCORE;
    }

    [[nodiscard]] std::array<size_t, 4> scores() const {
        return scores_;
    }

private:
    std::array<size_t, 4> scores_;

};


#endif //SCORE_H
