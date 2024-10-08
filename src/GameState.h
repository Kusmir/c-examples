#ifndef TRAMWAY_GAMESTATE_H
#define TRAMWAY_GAMESTATE_H

#include <vector>
#include <memory>
#include <array>

#include "GameEngine/Deck.h"
#include "GameRules.h"

class Player;

class GameState {
public:
    GameState(std::vector<std::shared_ptr<Player>> players = {},
              GameRules game_rules = GameRules(),
              long seed = std::chrono::system_clock::now().time_since_epoch().count());


    // setters
    void set_active_player(std::shared_ptr<Player> player);
    void set_round_id(int round_id) { _round_id = round_id; }
    void set_players(std::vector<std::shared_ptr<Player>> players);

    //getters
    Deck &deck();
    int round_id() const;
    int revealed_cards_count() const;
    const std::vector<std::shared_ptr<Player>> &players() const;
    const std::vector<std::shared_ptr<Player>> &starting_players() const;
    std::vector<std::vector<Card>> &table();
    const std::vector<std::vector<Card>> &table() const;
    std::shared_ptr<Player> active_player() const;
    size_t number_of_players() const;
    const Card &last_revealed_card() const;
    const GameRules &game_rules() const;
    size_t default_deck_size() const;

    void putCardOnTheTable(size_t position, Card card);
    void processNewCardInformation(Card card);
    void resetDeck();
    void resetTable();
    void collectCards();

private:
    std::vector<std::shared_ptr<Player>> _players;
    std::vector<std::shared_ptr<Player>> _starting_players;
    GameRules _game_rules;

    Deck _deck;
    int _round_id;
    std::vector<std::vector<Card>> _table;

    std::shared_ptr<Player> _active_player;
    int _revealed_cards_count = 0;
    Card _last_revealed_card;

    long _seed;
};


#endif //TRAMWAY_GAMESTATE_H
