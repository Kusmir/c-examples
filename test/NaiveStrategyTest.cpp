#include <gtest/gtest.h>
#include "GameState.h"
#include "GameEngine/Player/Strategy/CombinedStrategy/CombinedStrategy.h"
#include "GameEngine/Player/Player.h"

class NaiveStrategyTest_FirstRound: public ::testing::Test {
protected:
    void SetUp() override {
        p_player = std::make_shared<Player>("Player", CombinedStrategy::p_NaiveStrategy());
        game_state.set_round_id(1);
        game_state.set_active_player(p_player);
    }

    std::shared_ptr<Player> p_player;
    GameState game_state;
};


TEST_F(NaiveStrategyTest_FirstRound, testFirstDecision) {
    ASSERT_EQ(Decision::RED, p_player->makeDecision(game_state));
}

TEST_F(NaiveStrategyTest_FirstRound, shouldChooseHigher) {
    for (Card::Suit suit : {Card::Suit::CLUBS, Card::Suit::DIAMONDS, Card::Suit::HEARTS, Card::Suit::SPADES}) {
        for (int value_int = Card::Value::TWO; value_int <= Card::Value::SEVEN; ++value_int) {
            auto value = Card::Value(value_int);
            Card card(value, suit);

            p_player->takeCard(card);
            ASSERT_EQ(Decision::HIGHER, p_player->makeDecision(game_state));
            p_player->clearHand();
        }
    }
}

TEST_F(NaiveStrategyTest_FirstRound, shouldChooseLower) {
    for (Card::Suit suit : {Card::Suit::CLUBS, Card::Suit::DIAMONDS, Card::Suit::HEARTS, Card::Suit::SPADES}) {
        for (int value_int = Card::Value::EIGHT; value_int <= Card::Value::ACE; ++value_int) {
            auto value = Card::Value(value_int);
            Card card(value, suit);

            p_player->takeCard(card);
            ASSERT_EQ(Decision::LOWER, p_player->makeDecision(game_state));
            p_player->clearHand();
        }
    }
}

TEST_F(NaiveStrategyTest_FirstRound, testInsideOrOutside) {
    for (int left = Card::Value::TWO; left <= Card::Value::ACE; ++left) {
        for (int right = left; right <= Card::Value::ACE; ++right) {
            p_player->takeCard(Card(Card::Value(left), Card::Suit::SPADES));
            p_player->takeCard(Card(Card::Value(right), Card::Suit::HEARTS));

            int segment_length = right - left + 1;
            int half = (Card::Value::ACE - Card::Value::TWO + 1) / 2;

            Decision decision = p_player->makeDecision(game_state);
            Decision expected_decision = segment_length >= half ? Decision::INSIDE : Decision::OUTSIDE;

            ASSERT_EQ(expected_decision, decision);

            p_player->clearHand();
        }
    }
}

TEST_F(NaiveStrategyTest_FirstRound, shouldSwapOrderOfValuesIfNeeded) {
    p_player->takeCard(Card(Card::Value::JACK, Card::Suit::DIAMONDS));
    p_player->takeCard(Card(Card::Value::THREE, Card::Suit::HEARTS));

    ASSERT_EQ(Decision::INSIDE, p_player->makeDecision(game_state));
}

class NaiveStrategyTest_SecondRound : public ::testing::Test {
protected:
    void SetUp() override {
        p_player = std::make_shared<Player>("Player", CombinedStrategy::p_NaiveStrategy());
        p_player->takeCard(Card(Card::Value::FOUR, Card::Suit::DIAMONDS));
        p_player->takeCard(Card(Card::Value::FIVE, Card::Suit::CLUBS));
        p_player->takeCard(Card(Card::Value::JACK, Card::Suit::DIAMONDS));

        game_state.table().resize(1);
        game_state.set_round_id(2);
        game_state.set_active_player(p_player);
    }

    std::shared_ptr<Player> p_player;
    GameState game_state;
};

TEST_F(NaiveStrategyTest_SecondRound, shouldPlaceIfHasCardOfNeededValue) {
    game_state.putCardOnTheTable(0, Card(Card::Value::FIVE, Card::Suit::HEARTS));

    ASSERT_FALSE(game_state.last_revealed_card().value() == Card::Value::NONE);
    ASSERT_EQ(Decision::PLACE, p_player->makeDecision(game_state));
}

TEST_F(NaiveStrategyTest_SecondRound, shouldHoldIfDoesNotHaveCardOfNeededValue) {
    game_state.putCardOnTheTable(0, Card(Card::Value::SEVEN, Card::Suit::DIAMONDS));

    ASSERT_EQ(Decision::HOLD, p_player->makeDecision(game_state));
}

class NaiveStrategyTest_ThirdRound: public ::testing::Test {
protected:
    void SetUp() override {
        p_player = std::make_shared<Player>("Player", CombinedStrategy::p_NaiveStrategy());
        game_state.set_round_id(3);
        game_state.table().resize(1);
        game_state.set_active_player(p_player);
    }

    std::shared_ptr<Player> p_player;
    GameState game_state;
};

TEST_F(NaiveStrategyTest_ThirdRound, shouldChooseHigher) {
    for (Card::Suit suit : {Card::Suit::CLUBS, Card::Suit::DIAMONDS, Card::Suit::HEARTS, Card::Suit::SPADES}) {
        for (int value_int = Card::Value::TWO; value_int <= Card::Value::SEVEN; ++value_int) {
            auto value = Card::Value(value_int);
            Card card(value, suit);

            game_state.table().emplace_back();
            game_state.putCardOnTheTable(0, card);
            ASSERT_EQ(Decision::HIGHER, p_player->makeDecision(game_state));
            game_state.collectCards();
        }
    }
}

TEST_F(NaiveStrategyTest_ThirdRound, shouldChooseLower) {
    for (Card::Suit suit : {Card::Suit::CLUBS, Card::Suit::DIAMONDS, Card::Suit::HEARTS, Card::Suit::SPADES}) {
        for (int value_int = Card::Value::EIGHT; value_int <= Card::Value::ACE; ++value_int) {
            auto value = Card::Value(value_int);
            Card card(value, suit);

            game_state.table().emplace_back();
            game_state.putCardOnTheTable(0, card);
            ASSERT_EQ(Decision::LOWER, p_player->makeDecision(game_state));
            game_state.collectCards();
        }
    }
}
