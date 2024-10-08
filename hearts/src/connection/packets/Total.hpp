#ifndef TOTAL_H
#define TOTAL_H

#include <array>

#include "Packet.h"

class Total : public Packet {
public:
    [[nodiscard]] std::string get_message() const override {
        std::string ret = "TOTAL";
        for (int seat = 0; seat < (int)scores_.size(); ++seat) {
            ret += std::string(1, seat_to_char(seat));
            ret += std::to_string(scores_[seat]);
        }

        return ret + "\r\n";
    };

    explicit Total(const std::array<size_t, 4> &scores)
        : scores_(scores) {
        type_ = TOTAL;
    }

    [[nodiscard]] std::array<size_t, 4> scores() const {
        return scores_;
    }

private:
    std::array<size_t, 4> scores_;

};



#endif //TOTAL_H
