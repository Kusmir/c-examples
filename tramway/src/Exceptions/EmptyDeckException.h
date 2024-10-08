#ifndef TRAMWAY_EMPTYDECKEXCEPTION_H
#define TRAMWAY_EMPTYDECKEXCEPTION_H

#include <exception>

class EmptyDeckException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Attempt to deal a card from an empty deck.";
    }
};

#endif //TRAMWAY_EMPTYDECKEXCEPTION_H
