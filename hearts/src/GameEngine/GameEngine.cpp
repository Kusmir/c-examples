#include "GameEngine.h"
#include "../exceptions.hpp"
#include "../utils/parser.h"

#include <algorithm>
#include <cassert>



game_engine::GameEngine::GameEngine(const std::string &description_path)
    : active_player_seat_(NONE), first_seat_(NONE),
      deal_id_(1), finished_(false) {

    description_ = std::ifstream(description_path);
    for (Seat seat : {N, E, S, W}) {
        Player &player = players_[seat];
        player.seat = seat;
        player.cards = {};
        player.taken_tricks = {};
        player.total_score = 0;
        player.active = false;
    }
    if (!description_.is_open()) {
        throw InvalidFile(description_path);
    }
}

Seat game_engine::GameEngine::determine_winner() {
    assert(current_trick_.cards.size() == 4);
    Card::Color trick_color = current_trick_.cards[0].color();

    size_t best_id = 0;
    Card::Value current_best = current_trick_.cards[0].value();
    for (size_t id = 1; id < players_.size(); ++id) {
        const Card &card = current_trick_.cards[id];
        if (card.color() == trick_color and card.value() > current_best) {
            best_id = id;
            current_best = card.value();
        }
    }

    Seat winner_seat = Seat((best_id + active_player_seat_) % players_.size());
    players_[winner_seat].take_trick(current_trick_);
    active_player_seat_ = winner_seat;

    current_trick_.id++;
    current_trick_.cards.clear();

    return winner_seat;
}

std::array<std::vector<game_engine::Card>, game_engine::GameEngine::PLAYERS>
game_engine::GameEngine::start_new_deal() {
    if (description_.eof()) {
        return {};
    }

    int rules_int;
    description_ >> rules_int;
    rules_ = Rules(rules_int);

    char active_seat_char;
    description_ >> active_seat_char;
    first_seat_ = char_to_seat(active_seat_char);
    active_player_seat_ = first_seat_;

    std::array<std::vector<Card>, PLAYERS> hands;
    for (Seat seat: {N, E, S, W}) {
        std::string s;
        description_ >> s;
        players_[seat].cards = parser::parse_cards(s);
        hands[seat] = players_[seat].cards;
    }

    current_trick_ = Trick{
        .id = 1,
        .cards = {}
    };

    return hands;
}

std::array<size_t, 4> game_engine::GameEngine::end_deal() {
    assert(current_trick_.id > MAX_TRICKS);

    std::array<size_t, 4> trick_scores;
    for (Player &player: players_) {
        size_t score = player.get_score(rules_);
        player.total_score += score;
        trick_scores[player.seat] = score;

        player.taken_tricks.clear();
    }

    deal_id_++;
    return trick_scores;
}

std::array<size_t, 4> game_engine::GameEngine::get_total_scores() {
    std::array<size_t, 4> result{};
    for (Seat seat: {N, E, S, W})
        result[seat] = players_[seat].total_score;
    finished_ = true;
    active_player_seat_ = NONE;
    return result;
}

bool game_engine::GameEngine::place_card(Seat seat, Card card) {
    if (current_trick_.cards.size() == players_.size())
        // All the cards were already placed
        return false;

    if (active_player_seat_ != seat)
        // The player attempts to place a card but it's not their turn.
        return false;

    Player &active_player = players_[active_player_seat_];
    if (!active_player.has_card(card))
        return false;

    if (!current_trick_.cards.empty()) {
        Card::Color trick_color = current_trick_.cards[0].color();
        if (card.color() != trick_color
            and active_player.has_color(trick_color)) {
            return false;
        }
    }

    current_trick_.cards.emplace_back(card);
    active_player.remove_card(card);
    active_player_seat_ = Seat((active_player_seat_ + 1) % players_.size());
    return true;
}

std::array<bool, 4> game_engine::GameEngine::taken_seats() const {
    std::array<bool, 4> taken{};
    for (Seat seat: {N, E, S, W}) {
        taken[seat] = players_[seat].active;
    }
    return taken;
}

bool game_engine::GameEngine::is_seat_taken(Seat seat) const {
    return players_[seat].active;
}

bool game_engine::GameEngine::add_player(Seat seat) {
    if (players_[seat].active) return false;
    return players_[seat].active = true;
}

bool game_engine::GameEngine::remove_player(Seat seat) {
    if (!players_[seat].active) return false;
    players_[seat].active = false;
    return true;
}

size_t game_engine::GameEngine::active_players() const {
    size_t count = 0;
    for (Seat seat: {N, E, S, W})
        count += players_[seat].active;
    return count;
}

bool game_engine::GameEngine::paused() const {
    return finished_ or active_players() < players_.size();
}
