#include <gtest/gtest.h>

#include "GameEngine/Card.h"


class CardTest
        : public testing::TestWithParam<std::tuple<Card, std::string>> {};

TEST_F(CardTest, testValueEnumerator) {
    Card card1(Card::Value::ACE, Card::Suit::CLUBS);
    Card card2(Card::Value::TWO, Card::Suit::HEARTS);

    ASSERT_GT(card1.value(), card2.value());
    ASSERT_EQ(card1.value(), 12);
    ASSERT_EQ(card2.value(), 0);
}

TEST_P(CardTest, ToString) {
    auto [card, expectedResult] = GetParam();
    ASSERT_EQ(expectedResult, card.toString());
}

INSTANTIATE_TEST_SUITE_P(
        CardTest,
        CardTest,
        ::testing::Values(
                std::make_tuple(Card(Card::Value::SEVEN, Card::Suit::SPADES), " 7♠"),
                std::make_tuple(Card(Card::Value::TEN, Card::Suit::CLUBS), "10♣"),
                std::make_tuple(Card(Card::Value::JACK, Card::Suit::HEARTS)," J♥" ),
                std::make_tuple(Card(Card::Value::QUEEN, Card::Suit::DIAMONDS), " Q♦"),
                std::make_tuple(Card(Card::Value::KING, Card::Suit::DIAMONDS), " K♦"),
                std::make_tuple(Card(Card::Value::ACE, Card::Suit::SPADES), " A♠")));
