#include <iostream>
#include "GameEngine.h"
#include "Decision.h"
#include "Player/Player.h"
#include "../GameState.h"
#include "../Reporter/Reporter.h"
#include "../Exceptions/InvalidGameStateException.h"

GameEngine::GameEngine(long seed) : _rng(seed) {}

GameReport GameEngine::run(GameState &game_state) {
    runFirstRound(game_state);
    runSecondRound(game_state);

    while (game_state.players().size() != 1)
        runOvertime(game_state);

    runThirdRound(game_state);

    GameReport game_report = Reporter::prepareGameReport(game_state);
    _visualizer.displayEndScreen(game_report);
    return game_report;
}

// Evaluates correct decision for first round.
Decision correctDecision(GameState &game_state, int deal, Card card) {
    switch (deal) {
        case 1:
            if (card.suit() == Card::Suit::SPADES or card.suit() == Card::Suit::CLUBS)
                return Decision::BLACK;
            else
                return Decision::RED;
        case 2: {
            Card::Value players_value = game_state.active_player()->hand()[0].value();
            if (card.value() < players_value)
                return Decision::LOWER;
            else if (card.value() > players_value)
                return Decision::HIGHER;
            else
                return Decision::EQUAL;
        }
        case 3: {
            Card::Value left = game_state.active_player()->hand()[0].value();
            Card::Value right = game_state.active_player()->hand()[1].value();
            if (left > right) std::swap(left, right);

            if (left <= card.value() and card.value() <= right)
                return Decision::INSIDE;
            else
                return Decision::OUTSIDE;
        }
        default:
            throw InvalidGameStateException("Invalid round number.");
    }

    game_state.collectCards();
}

void GameEngine::runFirstRound(GameState &game_state, bool overtime) {
    game_state.set_round_id(1);
    game_state.table().resize(game_state.number_of_players());

    Deck &deck = game_state.deck();
    deck.shuffle();

    for (int deal = 1; deal <= 3; ++deal) {
        int player_index = 0;
        for (std::shared_ptr<Player> p_player: game_state.players()) {
            game_state.set_active_player(p_player);
            Card card = deck.dealCard();

            _visualizer.display(game_state);

            Decision players_decision = p_player->makeDecision(game_state);
            Decision correct_decision = correctDecision(game_state, deal, card);

            if (players_decision != correct_decision)
                p_player->takeStrike(game_state.round_id(), 1, overtime);

            game_state.putCardOnTheTable(player_index, card);
            p_player->takeCard(card);
            ++player_index;

            _visualizer.displayDecision(game_state, players_decision, correct_decision);
        }
    }

    if (overtime)
        endOvertime(game_state);
    else
        game_state.collectCards();
}

void buildPyramid(GameState &game_state) {
    Deck &deck = game_state.deck();
    int max_level = game_state.game_rules().max_second_round_level;
    game_state.table().resize((max_level * (max_level + 1)) / 2);

    int position = 0;
    for (int level = 0; level < max_level; ++level) {
        for (int i = 0; i < max_level - level; ++i) {
            Card card = deck.dealCard().faceDown();
            game_state.putCardOnTheTable(position, card);
            ++position;
        }
    }
}

// Requires game_state to represent state after first round.
// In explicit, deck is already declared, each player has three cards in hand
// and all other fields are empty.
void GameEngine::runSecondRound(GameState &game_state) {
    game_state.set_round_id(2);
    buildPyramid(game_state);

    int max_level = game_state.game_rules().max_second_round_level;
    int position = 0;
    for (int level = 1; level <= max_level; ++level) {
        for (int i = 0; i < max_level - level + 1; ++i) {
            Card &card = game_state.table()[position].back().faceUp();
            game_state.processNewCardInformation(card);

            std::vector<std::shared_ptr<Player>> can_place_card;
            for (std::shared_ptr<Player> p_player : game_state.players()) {
                _visualizer.display(game_state);
                Decision decision = p_player -> makeDecision(game_state);
                if (p_player->hasCardOfValue(card.value()) and decision == Decision::PLACE) {
                    can_place_card.push_back(p_player);
                }
            }

            if (!can_place_card.empty()) {
                int placing_player_index = std::uniform_int_distribution<int>(0, (int) can_place_card.size() - 1)(_rng);
                std::shared_ptr<Player> p_placing_player = can_place_card[placing_player_index];
                Card placed_card = p_placing_player->giveCardOfValue(card.value());
                game_state.putCardOnTheTable(position, placed_card);

                std::shared_ptr<Player> p_punished_player = nullptr;
                if (level % 2 == 1) { // Placing player gets strike.
                    p_punished_player = p_placing_player;
                } else if (game_state.players().size() > 1) { // Another random player gets strike.
                    int punished_player_index =
                            std::uniform_int_distribution<int>(0, (int) game_state.players().size() - 2)(_rng);
                    p_punished_player = game_state.players()[punished_player_index];
                    if (p_punished_player == p_placing_player)
                        p_punished_player = game_state.players().back();
                }

                if (p_punished_player != nullptr) {
                    p_punished_player->takeStrike(game_state.round_id(), level);
                    _visualizer.displayPunishment(game_state, p_punished_player, level);
                }
            }
            ++position;
        }
    }

    endSecondRound(game_state);
}

void GameEngine::endSecondRound(GameState &game_state) {
    std::vector<std::shared_ptr<Player>> losers;
    size_t max_left_cards = 0;
    for (std::shared_ptr<Player> p_player : game_state.players()) {
        max_left_cards = std::max(max_left_cards, p_player->hand().size());
    }

    for (std::shared_ptr<Player> p_player : game_state.players()) {
        if (p_player->hand().size() == max_left_cards)
            losers.push_back(p_player);
    }

    for (std::shared_ptr<Player> p_player : game_state.players())
        p_player->clearHand();

    game_state.set_players(losers);
    game_state.collectCards();
    game_state.resetDeck();
}

bool dealCardsForThirdRound(GameState &game_state) {
    std::vector<std::vector<Card>> &table = game_state.table();
    int max_cards_in_final = game_state.game_rules().max_cards_in_final;
    for (int position = 0; position < max_cards_in_final; ++position) {
        if (!table[position].empty() and table[position].back().suit() == Card::Suit::CARD_BACK)
            return true;

        if (game_state.deck().empty())
            return false;

        Card card = game_state.deck().dealCard();
        game_state.putCardOnTheTable(position, card.faceDown());
    }

    return true;
}

Decision compareCards(Card card1, Card card2) {
    if (card1.value() < card2.value())
        return Decision::LOWER;
    else if (card1.value() > card2.value())
        return Decision::HIGHER;
    else
        return Decision::EQUAL;
}

void GameEngine::runThirdRound(GameState &game_state, int reshuffles) {
    game_state.set_round_id(3);
    Deck &deck = game_state.deck();
    deck.shuffle();

    game_state.table().resize(game_state.game_rules().max_cards_in_final);
    dealCardsForThirdRound(game_state);

    std::vector<std::vector<Card>> &table = game_state.table();

    std::shared_ptr<Player> p_player = game_state.players()[0];
    game_state.set_active_player(p_player);

    int current_position = 0;
    while (current_position < game_state.game_rules().max_cards_in_final) {
        _visualizer.display(game_state);
        if (current_position == 0) {
            Card current_card = table[0].back().faceUp();
            game_state.processNewCardInformation(current_card);

            ++current_position;
            continue;
        }

        Decision players_decision = p_player->makeDecision(game_state);

        Card &current_card = table[current_position].back().faceUp();
        game_state.processNewCardInformation(current_card);

        Card previous_card = table[current_position - 1].back();
        Decision correct_decision = compareCards(current_card, previous_card);

        _visualizer.displayDecision(game_state, players_decision, correct_decision);

        if (players_decision != correct_decision) {
            p_player->takeStrike(game_state.round_id());
            current_position = 0;
            bool can_continue = dealCardsForThirdRound(game_state);
            if (!can_continue)
                break;
        } else {
            ++current_position;
        }
    }

    if (current_position < game_state.game_rules().max_cards_in_final
        and reshuffles < game_state.game_rules().reshuffle_in_final) {

        game_state.collectCards();
        game_state.resetDeck();
        runThirdRound(game_state, ++reshuffles);
    }
}

void GameEngine::runOvertime(GameState &game_state) {
    runFirstRound(game_state, true);
}

void GameEngine::endOvertime(GameState &game_state) {
    int min_points = 3;
    for (std::shared_ptr<Player> p_player : game_state.players())
        min_points = std::min(min_points, p_player->overtime_points());

    std::vector<std::shared_ptr<Player>> losers;
    for (std::shared_ptr<Player> p_player : game_state.players()) {
        if (p_player->overtime_points() == min_points)
            losers.push_back(p_player);

        p_player->set_overtime_points(0);
        p_player->clearHand();
    }

    game_state.set_players(losers);
    game_state.collectCards();
}

void GameEngine::set_visualize(bool visualize) {
    _visualizer.set_active(visualize);
}

