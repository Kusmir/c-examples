#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <utility>
#include <cstring>

class InvalidMessageException : public std::exception {
    std::string message_;

public:
    std::string message() { return message_; }

    explicit InvalidMessageException(const std::string &message)
        : message_(message) {
    }

    const char *what() const noexcept override {
        return message_.data();
    }
};

class SysFailed : public std::exception {
    std::string message_;

public:
    explicit SysFailed(std::string name) {
        message_ = name + ": [errno " + std::to_string(errno) + "] "
                   + std::strerror(errno);
    }

    const char *what() const noexcept override { return message_.data(); }
};

class InvalidFile : public std::exception {
    std::string message_;

public:
    explicit InvalidFile(std::string path) {
        message_ = "Could not open file: " + path;
    }

    const char *what() const noexcept override { return message_.data(); }
};

class ConnectionClosed : public std::exception {
public:
    const char *what() const noexcept override {
        return "Connection closed by peer.";
    }
};

#endif //EXCEPTIONS_H
