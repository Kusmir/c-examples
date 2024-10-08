#ifndef TRAMWAY_PLAYER_H
#define TRAMWAY_PLAYER_H


#include <vector>
#include "../Card.h"
#include "Strategy/CombinedStrategy/CombinedStrategy.h"
#include "../Hand.h"

class Player {
public:
    Player(std::string name, std::shared_ptr<IStrategy> strategy);

    Decision makeDecision(const GameState &game_state);
    void takeCard(Card card);
    bool hasCardOfValue(Card::Value value);
    Card giveCardOfValue(Card::Value value);
    void takeStrike(int round_id, int strikes = 1, bool overtime = false);
    void clearHand() { _hand.clear(); };

    std::string name();
    const Hand& hand() const;
    int strike_counter() const;
    int overtime_points() const;
    const std::string& name() const;
    const std::array<int, 3> round_scores() const;

    void set_overtime_points(int points) { _overtime_points = points; }

private:
    std::string _name;
    std::shared_ptr<IStrategy> _strategy;
    Hand _hand;

    std::array<int, 3> _round_scores;
    int _overtime_points = 0;
};


#endif //TRAMWAY_PLAYER_H
