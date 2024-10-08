#ifndef SERVER_OPTIONS_H
#define SERVER_OPTIONS_H
#include <cstdint>
#include <string>


namespace server_options {
    struct ProgramOptions {
        uint16_t port;
        std::string input_path;
        size_t timeout;
    };

    ProgramOptions get_program_options(int argc, char* argv[]);
    void log_program_options(const ProgramOptions& options);
}

#endif //SERVER_OPTIONS_H
