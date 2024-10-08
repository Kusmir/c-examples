#include <iostream>
#include "InteractiveStrategy.h"
#include "../../../Exceptions/InvalidGameStateException.h"

Decision redOrBlack() {
    while (true) {
        std::cout << "Red or black?\n";
        std::string decision;
        std::cin >> decision;

        if (decision == "red") {
            std::cin.get();
            return Decision::RED;
        } if (decision == "black") {
            std::cin.get();
            return Decision::BLACK;
        }

        std::cout << "Invalid input. Try again.\n";
    }
}

Decision higherOrLower() {
    while (true) {
        std::cout << "Higher, lower or equal?\n";
        std::string decision;
        std::cin >> decision;

        if (decision == "higher") {
            std::cin.get();
            return Decision::HIGHER;
        } if (decision == "lower") {
            std::cin.get();
            return Decision::LOWER;
        } if (decision == "equal") {
            std::cin.get();
            return Decision::EQUAL;
        }

        std::cout << "Invalid input. Try again.\n";
    }
}

Decision insideOrOutside() {
    while (true) {
        std::cout << "Inside or outside?\n";
        std::string decision;
        std::cin >> decision;

        if (decision == "inside") {
            std::cin.get();
            return Decision::INSIDE;
        }
        if (decision == "outside") {
            std::cin.get();
            return Decision::OUTSIDE;
        }

        std::cout << "Invalid input. Try again.\n";
    }
    std::cin.get();
}

Decision wantToPlace() {
    while (true) {
        std::cout << "You have a card of required value in your hand. Do you want to place it? (place / hold)\n";
        std::string decision;
        std::cin >> decision;

        if (decision == "place") {
            std::cin.get();
            return Decision::PLACE;
        } if (decision == "hold") {
            std::cin.get();
            return Decision::HOLD;
        }

        std::cout << "Invalid input. Try again.\n";
    }
}

Decision InteractiveStrategy::makeDecision(const GameState &game_state, const Hand &hand) {
    if (game_state.round_id() == 1) {
        switch (hand.size()) {
            case 0:
               return redOrBlack();
            case 1:
                return higherOrLower();
            case 2:
                return insideOrOutside();
            default:
                throw InvalidGameStateException("Player has too many cards in hand.");
        }
    } else if (game_state.round_id() == 2) {
        Card::Value card_value = game_state.last_revealed_card().value();
        if (hand.containsValue(card_value))
            return wantToPlace();
        else {
            std::cout << "You don't have a card to place.\n";
            std::cin.get();
            return Decision::HOLD;
        }
    } else {
        return higherOrLower();
    }
}
