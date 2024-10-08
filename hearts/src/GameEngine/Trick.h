#ifndef TRICK_H
#define TRICK_H

namespace game_engine {
    struct Trick {
        size_t id = 0;
        std::vector<Card> cards;
    };
}

#endif //TRICK_H
