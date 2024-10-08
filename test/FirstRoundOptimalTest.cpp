#include <gtest/gtest.h>
#include "GameEngine/Hand.h"
#include "GameState.h"
#include "GameEngine/Decision.h"
#include "GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundOptimal.h"
#include "GameEngine/Player/Strategy/IStrategy.h"
#include "GameEngine/Player/Strategy/CombinedStrategy/CombinedStrategy.h"
#include "GameEngine/Player/Player.h"


class FirstRoundOptimal_FirstDealTest : public ::testing::Test {
protected:
    class SecondRoundEmpty : public SecondRoundStrategy {
    private:
        Decision shouldPlaceCard(const GameState &game_state[[maybe_unused]], const Hand &hand[[maybe_unused]]) override {
            return Decision::HIGHER;
        }
    };

    class ThirdRoundEmpty : public ThirdRoundStrategy {
    public:
        Decision makeDecision(const GameState &game_state[[maybe_unused]], const Hand &hand[[maybe_unused]]) override {
            return Decision::HIGHER;
        }
    };

    void SetUp() override {
        std::shared_ptr<IStrategy> p_strategy = std::make_shared<CombinedStrategy>(CombinedStrategy(
            std::make_shared<FirstRoundOptimal>(FirstRoundOptimal()),
            std::make_shared<SecondRoundEmpty>(SecondRoundEmpty()),
            std::make_shared<ThirdRoundEmpty>(ThirdRoundEmpty())
        ));

        std::vector<std::shared_ptr<Player>> players;
        for (int i = 1; i <= 4; ++i) {
            std::shared_ptr<Player> p_player = std::make_shared<Player>(
                    Player("Player" + std::to_string(i), p_strategy));

            players.push_back(p_player);
        }

        game_state = GameState(players);
        game_state.set_round_id(1);
        game_state.table().resize(game_state.number_of_players());
    }

    GameState game_state;
};

TEST_F(FirstRoundOptimal_FirstDealTest, shouldChooseRed) {
    std::vector<std::shared_ptr<Player>> players = game_state.players();

    std::vector<Card> dealt_so_far = {
            Card(Card::Value::EIGHT, Card::Suit::CLUBS),
            Card(Card::Value::TWO, Card::Suit::SPADES),
            Card(Card::Value::THREE, Card::Suit::DIAMONDS)
    };

    for (size_t i = 0; i < dealt_so_far.size(); ++i) {
        Card card = dealt_so_far[i];
        std::shared_ptr<Player> p_player = players[i];

        game_state.putCardOnTheTable(i, card);
        p_player->takeCard(card);
    }

    std::shared_ptr<Player> p_player = players[3];
    ASSERT_EQ(Decision::RED, p_player->makeDecision(game_state));
}

TEST_F(FirstRoundOptimal_FirstDealTest, shouldChooseBlack) {
    std::vector<std::shared_ptr<Player>> players = game_state.players();

    std::vector<Card> dealt_so_far = {
            Card(Card::Value::EIGHT, Card::Suit::HEARTS),
            Card(Card::Value::TWO, Card::Suit::SPADES),
            Card(Card::Value::THREE, Card::Suit::DIAMONDS)
    };

    for (size_t i = 0; i < dealt_so_far.size(); ++i) {
        Card card = dealt_so_far[i];
        std::shared_ptr<Player> p_player = players[i];

        game_state.putCardOnTheTable(i, card);
        p_player->takeCard(card);
    }

    std::shared_ptr<Player> p_player = players[3];
    ASSERT_EQ(Decision::BLACK, p_player->makeDecision(game_state));
}

TEST_F(FirstRoundOptimal_FirstDealTest, shouldChooseRedOrBlackWhenDecidingFirst) {
    std::shared_ptr<Player> p_player = game_state.players()[0];
    Decision decision = p_player->makeDecision(game_state);
    ASSERT_TRUE(decision == Decision::BLACK or decision == Decision::RED);
}

TEST_F(FirstRoundOptimal_FirstDealTest, shouldChooseRedOrBlackWhenBothAreOptimal) {
    std::vector<std::shared_ptr<Player>> players = game_state.players();

    std::vector<Card> dealt_so_far = {
            Card(Card::Value::SIX, Card::Suit::HEARTS),
            Card(Card::Value::JACK, Card::Suit::CLUBS),
    };

    for (size_t i = 0; i < dealt_so_far.size(); ++i) {
        Card card = dealt_so_far[i];
        std::shared_ptr<Player> p_player = players[i];

        game_state.putCardOnTheTable(i, card);
        p_player->takeCard(card);
    }

    std::shared_ptr<Player> p_player = players[2];
    Decision decision = p_player->makeDecision(game_state);
    ASSERT_TRUE(decision == Decision::BLACK or decision == Decision::RED);
}


class FirstRoundOptimal_SecondDealTest : public FirstRoundOptimal_FirstDealTest {
protected:
    void SetUp() override {
        FirstRoundOptimal_FirstDealTest::SetUp();
        std::vector<std::shared_ptr<Player>> players = game_state.players();

        std::vector<Card> cards_to_deal = {
                Card(Card::Value::SIX, Card::Suit::HEARTS),
                Card(Card::Value::EIGHT, Card::Suit::HEARTS),
                Card(Card::Value::JACK, Card::Suit::CLUBS),
                Card(Card::Value::TWO, Card::Suit::SPADES),
        };

        // Reverse so cards are dealt in order in which they were initialized.
        std::reverse(cards_to_deal.begin(), cards_to_deal.end());

        for (size_t i = 0; i < game_state.number_of_players(); ++i) {
            Card card = cards_to_deal.back();
            cards_to_deal.pop_back();
            std::shared_ptr<Player> p_player = players[i];

            game_state.putCardOnTheTable(i, card);
            p_player->takeCard(card);
        }
    }
};

TEST_F(FirstRoundOptimal_SecondDealTest, shouldChooseHigher) {
    ASSERT_EQ(Decision::HIGHER, game_state.players()[0]->makeDecision(game_state));
    ASSERT_EQ(Decision::HIGHER, game_state.players()[3]->makeDecision(game_state));
}

TEST_F(FirstRoundOptimal_SecondDealTest, shouldChooseLower) {
    ASSERT_EQ(Decision::LOWER, game_state.players()[1]->makeDecision(game_state));
    ASSERT_EQ(Decision::LOWER, game_state.players()[2]->makeDecision(game_state));
}

class FirstRoundOptimal_ThirdDealTest : public FirstRoundOptimal_SecondDealTest {
protected:
    void SetUp() override {
        FirstRoundOptimal_SecondDealTest::SetUp();
        std::vector<std::shared_ptr<Player>> players = game_state.players();

        std::vector<Card> cards_to_deal = {
                Card(Card::Value::TEN, Card::Suit::SPADES),
                Card(Card::Value::TWO, Card::Suit::CLUBS),
                Card(Card::Value::ACE, Card::Suit::HEARTS),
                Card(Card::Value::EIGHT, Card::Suit::SPADES),
        };

        // Reverse so cards are dealt in order in which they were initialized.
        std::reverse(cards_to_deal.begin(), cards_to_deal.end());

        for (size_t i = 0; i < game_state.number_of_players(); ++i) {
            Card card = cards_to_deal.back();
            cards_to_deal.pop_back();
            std::shared_ptr<Player> p_player = players[i];

            game_state.putCardOnTheTable(i, card);
            p_player->takeCard(card);
        }
    }
};

TEST_F(FirstRoundOptimal_ThirdDealTest, shouldChooseInside) {
    EXPECT_EQ(Decision::INSIDE, game_state.players()[1]->makeDecision(game_state));
    EXPECT_EQ(Decision::INSIDE, game_state.players()[3]->makeDecision(game_state));
}

TEST_F(FirstRoundOptimal_ThirdDealTest, shouldChooseOutside) {
    EXPECT_EQ(Decision::OUTSIDE, game_state.players()[0]->makeDecision(game_state));
    EXPECT_EQ(Decision::OUTSIDE, game_state.players()[2]->makeDecision(game_state));
}
