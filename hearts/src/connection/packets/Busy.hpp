#ifndef BUSY_H
#define BUSY_H
#include <array>

#include "Packet.h"
#include "../../common.h"


class Busy : public Packet {
public:
    explicit Busy(const std::array<bool, 4> &taken) {
        type_ = BUSY;
        taken_seats_ = {};
        for (int seat = 0; seat < (int)taken.size(); ++seat)
            if (taken[seat])
                taken_seats_.emplace_back(static_cast<Seat>(seat));
    }

    explicit Busy(const std::vector<Seat> &seats)
        : taken_seats_(seats) {
        type_ = BUSY;
    }

    [[nodiscard]] std::vector<Seat> taken_seats() const {
        return taken_seats_;
    }

    [[nodiscard]] std::string get_message() const override {
        std::string ret = "BUSY";
        for (Seat seat : taken_seats_)
            ret += seat_to_char(seat);
        return ret + "\r\n";
    };

private:
    std::vector<Seat> taken_seats_;
};


#endif //BUSY_H
