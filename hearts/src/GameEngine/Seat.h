#ifndef SEAT_H
#define SEAT_H

#include <cstddef>
#include <ostream>

enum Seat {
    N, E, S, W, NONE
};

constexpr size_t SEATS = 4;
char seat_to_char(int seat);
Seat char_to_seat(char c);
std::ostream &operator << (std::ostream &os, Seat seat);


#endif //SEAT_H
