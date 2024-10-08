#include "FirstRoundOptimal.h"

Decision FirstRoundOptimal::redOrBlack(const GameState &game_state) {
    int black_counter = 0;
    int red_counter = 0;

    for (const std::vector<Card> &pile : game_state.table()) {
        for (Card card : pile) {
            if (card.suit() == Card::Suit::DIAMONDS or card.suit() == Card::Suit::HEARTS)
                ++red_counter;
            else
                ++black_counter;
        }
    }

    return red_counter < black_counter ? Decision::RED : Decision::BLACK;
}

Decision FirstRoundOptimal::higherOrLower(const GameState &game_state, const Hand &hand) {
    // There are 13 suits in the deck. Always. I know it's a magic number, but nah, I don't want to.
    int default_deck_size = (int)game_state.default_deck_size();
    int default_card_value_count = default_deck_size / 13;
    Card my_card = hand[0];

    int lower = my_card.value() * default_card_value_count;
    int equal = default_card_value_count;
    int higher = default_deck_size - lower - equal;

    for (const std::vector<Card> &pile : game_state.table()) {
        if (pile.empty()) break;
        for (const Card &card : pile) {
            if (card.value() > hand[0].value())
                --lower;
            else if (card.value() < hand[0].value())
                --higher;
            else
                --equal;
        }
    }

    if (lower > higher and lower > equal)
        return Decision::LOWER;
    else if (higher > equal)
        return Decision::HIGHER;
    else
        return Decision::EQUAL;
}

Decision FirstRoundOptimal::insideOrOutside(const GameState &game_state, const Hand &hand) {
    int default_deck_size = (int)game_state.default_deck_size();
    int default_card_value_count = default_deck_size / 13;

    int segment_start = std::min(hand[0].value(), hand[1].value());
    int segment_end = std::max(hand[0].value(), hand[1].value());
    int inside = (segment_end - segment_start + 1) * default_card_value_count;
    int outside = default_deck_size - inside;

    for (const std::vector<Card> &pile : game_state.table()) {
        if (pile.empty()) break;
        for (const Card &card: pile) {
            if (segment_start <= card.value() and card.value() <= segment_end)
                --inside;
            else
                --outside;
        }
    }

    return inside > outside ? Decision::INSIDE : Decision::OUTSIDE;
}
