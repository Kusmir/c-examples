#include <gtest/gtest.h>
#include "GameEngine/Hand.h"


class HandTest : public ::testing::Test {
protected:
    void SetUp() override {
        hand = Hand({
            Card(Card::Value::TWO, Card::Suit::CLUBS),
            Card(Card::Value::ACE, Card::Suit::SPADES),
            Card(Card::Value::QUEEN, Card::Suit::HEARTS)
        });

        same_values_hand = Hand({
            Card(Card::Value::TWO, Card::Suit::CLUBS),
            Card(Card::Value::TWO, Card::Suit::SPADES),
        });
    }

    Hand empty_hand;
    Hand hand;
    Hand same_values_hand;
};

TEST_F(HandTest, testContainsValue) {
    Card::Value value = Card::Value::TWO;

    EXPECT_FALSE(empty_hand.containsValue(value));
    EXPECT_TRUE(hand.containsValue(value));
    EXPECT_TRUE(same_values_hand.containsValue(value));
}

TEST_F(HandTest, testRemove) {
    Card card(Card::Value::ACE, Card::Suit::SPADES);

    ASSERT_TRUE(hand.remove(card));

    EXPECT_TRUE(hand.size() == 2);

    EXPECT_FALSE(hand.containsValue(Card::Value::ACE));
    EXPECT_TRUE(hand.containsValue(Card::Value::QUEEN));
    EXPECT_TRUE(hand.containsValue(Card::Value::TWO));
}

TEST_F(HandTest, testAdd) {
    Card card(Card::Value::THREE, Card::Suit::DIAMONDS);
    empty_hand.add(card);

    ASSERT_TRUE(empty_hand.size() == 1);
    EXPECT_TRUE(empty_hand.containsValue(Card::Value::THREE));
}