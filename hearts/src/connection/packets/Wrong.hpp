#ifndef WRONG_H
#define WRONG_H
#include "Packet.h"


class Wrong : public Packet {
public:
    explicit Wrong(size_t trick_id)
        : trick_id_(trick_id) {
        type_ = WRONG;
    }

    [[nodiscard]] size_t trick_id() const {
        return trick_id_;
    }

    [[nodiscard]] std::string get_message() const override {
        return "WRONG" + std::to_string(trick_id_) + "\r\n";
    }

private:
    size_t trick_id_;

};



#endif //WRONG_H
