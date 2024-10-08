#include "Player.h"

bool game_engine::Player::has_card(Card card) {
    for (const Card &c : cards)
        if (c == card) return true;
    return false;
}

bool game_engine::Player::remove_card(Card card) {
    for (auto it = cards.begin(); it != cards.end(); it++) {
        if (*it == card) {
            cards.erase(it);
            return true;
        }
    }
    return false;
}

bool game_engine::Player::has_color(Card::Color color) {
    for (const Card &c: cards)
        if (c.color() == color) return true;
    return false;
}

void game_engine::Player::take_trick(const Trick &trick) {
    taken_tricks.emplace_back(trick);
}

std::vector<game_engine::Card> game_engine::Player::taken_cards() {
    std::vector<Card> all_cards = {};
    for (const Trick &trick: taken_tricks) {
        all_cards.insert(
            all_cards.end(),
            trick.cards.begin(),
            trick.cards.end()
        );
    }
    return all_cards;
}

size_t game_engine::Player::get_score(Rules rule) {
    std::vector<Card> all_cards = taken_cards();
    size_t score = 0;
    switch (rule) {
        case TRICKS:
            return taken_tricks.size();
        case HEARTS:
            for (const Card &card: all_cards)
                score += (card.color() == Card::Color::HEARTS);
            break;
        case QUEENS:
            for (const Card &card: all_cards)
                score += (card.value() == Card::Value::QUEEN) * 5;
            break;
        case JACKS_AND_KINGS:
            for (const Card &card: all_cards)
                score += (card.value() == Card::Value::JACK or
                          card.value() == Card::Value::KING) * 2;
            break;
        case KING_OF_HEARTS:
            for (const Card &card: all_cards)
                score += (card == Card(Card::Value::KING, Card::Color::HEARTS))
                        * 18;
            break;
        case SEVENTH_AND_LAST:
            for (const Trick &trick: taken_tricks)
                score += (trick.id == 7 or trick.id == 13) * 10;
            break;
        case ALL:
            for (Rules r: {
                     TRICKS, HEARTS, QUEENS, JACKS_AND_KINGS,
                     KING_OF_HEARTS, SEVENTH_AND_LAST
                 }) {
                score += get_score(r);
            }
    }

    return score;
}
