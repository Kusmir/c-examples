#ifndef TRAMWAY_REPORTER_H
#define TRAMWAY_REPORTER_H


#include "GameReport.h"
#include "../GameState.h"

class Reporter {
public:
    static GameReport prepareGameReport(const GameState &game_state);
};


#endif //TRAMWAY_REPORTER_H
