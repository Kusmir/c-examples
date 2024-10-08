#include "Player.h"

#include <utility>
#include <stdexcept>

void Player::takeCard(Card card) {
    _hand.add(card);
}

bool Player::hasCardOfValue(Card::Value value) {
    return _hand.containsValue(value);
}

Card Player::giveCardOfValue(Card::Value value) {
    // Choice of specific suit doesn't matter.
    for (Card card: _hand.content()) {
        if (card.value() == value) {
            _hand.remove(card);
            return card;
        }
    }

    // TODO: Proper exception.
    throw std::runtime_error("Player was forced to place card they don't own.");
}

void Player::takeStrike(int round_id, int strikes, bool overtime) {
    if (overtime)
        _overtime_points += strikes;
    else
        _round_scores[round_id - 1] += strikes;
}

Player::Player(std::string name, std::shared_ptr<IStrategy> strategy) : _name(std::move(name)),
                                                                        _strategy(std::move(strategy)) {
    _round_scores = {};
}

Decision Player::makeDecision(const GameState &game_state) {
    return _strategy->makeDecision(game_state, _hand);
}

std::string Player::name() {
    return _name;
}

const Hand &Player::hand() const {
    return _hand;
}

int Player::strike_counter() const {
    return std::accumulate(_round_scores.begin(), _round_scores.end(), 0);
}

int Player::overtime_points() const {
    return _overtime_points;
}

const std::string &Player::name() const {
    return _name;
}

const std::array<int, 3> Player::round_scores() const {
    return _round_scores;
}
