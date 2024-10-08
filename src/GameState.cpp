#include "GameState.h"

#include <utility>

GameState::GameState(std::vector<std::shared_ptr<Player>> players,
                     GameRules game_rules,
                     long seed)
        : _players(players),
          _starting_players(players),
          _game_rules(game_rules),
          _deck(game_rules.number_of_decks),
          _last_revealed_card(Card(Card::Value::NONE, Card::Suit::CARD_BACK)),
          _seed(seed) {}

void GameState::set_active_player(std::shared_ptr<Player> player) {
    _active_player = player;
}

void GameState::set_players(std::vector<std::shared_ptr<Player>> players) {
    _players = players;
}

void GameState::resetDeck() {
    _deck.reset();
}

void GameState::putCardOnTheTable(size_t position, Card card) {
    _table[position].push_back(card);
    if (card.suit() != Card::Suit::CARD_BACK)
        processNewCardInformation(card);
}

void GameState::processNewCardInformation(Card card) {
    _last_revealed_card = card;
    ++_revealed_cards_count;
}

void GameState::resetTable() {
    _table.clear();
}

void GameState::collectCards() {
    resetTable();
    _revealed_cards_count = 0;
}

Deck &GameState::deck() {
    return _deck;
}

int GameState::round_id() const {
    return _round_id;
}

int GameState::revealed_cards_count() const {
    return _revealed_cards_count;
}

const std::vector<std::shared_ptr<Player>> &GameState::players() const {
    return _players;
}

std::vector<std::vector<Card>> &GameState::table() {
    return _table;
}

const std::vector<std::vector<Card>> &GameState::table() const {
    return _table;
}

std::shared_ptr<Player> GameState::active_player() const {
    return _active_player;
}

size_t GameState::number_of_players() const {
    return _players.size();
}

const Card &GameState::last_revealed_card() const {
    return _last_revealed_card;
}

const GameRules &GameState::game_rules() const {
    return _game_rules;
}

const std::vector<std::shared_ptr<Player>> &GameState::starting_players() const {
    return _starting_players;
}

size_t GameState::default_deck_size() const {
    return _deck.default_size();
}
