#include <gtest/gtest.h>

#include "GameEngine/Deck.h"
#include "Exceptions/EmptyDeckException.h"

TEST(DeckTest, testConstructor) {
    Deck deck;

    ASSERT_EQ(52, deck.size());

    auto smallerCard = [](Card A, Card B) {
        return A.value() < B.value() or
              (A.value() == Card::Value::ACE and B.value() == Card::Value::TWO);
    };

    std::vector<int> counter(13);
    Card previous_card = deck.dealCard();
    counter[previous_card.value()]++;

    while (!deck.empty()) {
        Card current_card = deck.dealCard();
        counter[current_card.value()]++;
        ASSERT_TRUE(smallerCard(current_card, previous_card));
        previous_card = current_card;
    }

    for (int count : counter)
        ASSERT_EQ(4, count);
}

TEST(DeckTest, testShuffle) {
    Deck deck;
    deck.shuffle();

    ASSERT_EQ(52, deck.size());
}

TEST(DeckTest, shouldThrowExceptionWhenTryingToDealFromEmptyDeck) {
    Deck deck;
    EXPECT_THROW({
        try {
            while (true)
                deck.dealCard();
        } catch (const EmptyDeckException &e) {
            EXPECT_STREQ("Attempt to deal a card from an empty deck.",
                         e.what());
            throw;
        }
    }, EmptyDeckException);
}