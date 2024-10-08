#include "Seat.h"

char seat_to_char(const int seat) {
    switch (seat) {
        case N: return 'N';
        case S: return 'S';
        case E: return 'E';
        case W: return 'W';
        default: return '?';
    }
}

Seat char_to_seat(char c) {
    switch (c) {
        case 'N': return N;
        case 'S': return S;
        case 'E': return E;
        case 'W': return W;
        default: return NONE;
    }
}
std::ostream & operator << (std::ostream &os, Seat seat) {
    return os << seat_to_char(seat);
}
