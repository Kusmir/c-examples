#include <iostream>

#include "utils/Card.h"
//
// int main() {
//     // Card card(Card::Value::TWO, Card::Color::SPADES);
//     // std::cout << card.toString(true) << '\n';
//     // std::cout << card.toString() << "\n\n";
//     // return 0;
//
//     for (int color_int = Card::Color::HEARTS; color_int <= Card::Color::DIAMONDS; ++color_int) {
//         for (int value_int = Card::Value::TWO; value_int <= Card::Value::ACE; ++value_int) {
//             Card::Value value = static_cast<Card::Value>(value_int);
//             Card::Color color = static_cast<Card::Color>(color_int);
//             Card card(value, color);
//             std::cout << card.to_string(true) << '\n';
//             std::cout << card.to_string() << "\n\n";
//         }
//     }
// }