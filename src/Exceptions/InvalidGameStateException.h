#ifndef TRAMWAY_INVALIDGAMESTATEEXCEPTION_H
#define TRAMWAY_INVALIDGAMESTATEEXCEPTION_H

#include <exception>
#include <string>

class InvalidGameStateException : public std::exception {
private:
    std::string _message;
public:
    explicit InvalidGameStateException(std::string message = "") {
        _message = "Invalid game state. " + message;
    }

    const char* what() const noexcept override {
        return _message.c_str();
    }
};

#endif //TRAMWAY_INVALIDGAMESTATEEXCEPTION_H
