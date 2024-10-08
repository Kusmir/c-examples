#ifndef TAKEPLACE_H
#define TAKEPLACE_H

#include "Packet.h"
#include "../../common.h"


class TakePlace : public Packet {
public:
    [[nodiscard]] int seat() const {
        return seat_;
    }

    explicit TakePlace(int seat)
        : seat_(seat) {
        type_ = IAM;
    }

    [[nodiscard]] std::string get_message() const override {
        std::string s = "IAM" + std::string(1, seat_to_char(seat_)) + "\r\n";
        return s;
    }

private:
    int seat_;
};



#endif //TAKEPLACE_H
