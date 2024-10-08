#ifndef TRAMWAY_INVALIDPLAYERDECISIONEXCEPTION_H
#define TRAMWAY_INVALIDPLAYERDECISIONEXCEPTION_H

#include <exception>

class EmptyDeckException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Player made invalid decision.";
    }
};

#endif //TRAMWAY_INVALIDPLAYERDECISIONEXCEPTION_H
