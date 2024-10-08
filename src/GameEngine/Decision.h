#ifndef TRAMWAY_DECISION_H
#define TRAMWAY_DECISION_H

enum class Decision : int {
    LOWER = -1,
    EQUAL = 0,
    HIGHER = 1,
    RED,
    BLACK,
    INSIDE,
    OUTSIDE,
    PLACE,
    HOLD,
    NONE
};

#endif //TRAMWAY_DECISION_H
