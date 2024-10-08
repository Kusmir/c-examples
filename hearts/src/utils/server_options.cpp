//
// Created by kusmir on 09.06.24.
//

#include "server_options.h"

#include <iostream>
namespace {
    uint16_t read_port(char const *string) {
        char *endptr;
        errno = 0;
        unsigned long port = strtoul(string, &endptr, 10);
        if (errno != 0 || *endptr != 0 || port > UINT16_MAX) {
            std::cerr << string << " is not a valid port number.\n";
            exit(1);
        }
        return (uint16_t) port;
    }
}

server_options::ProgramOptions
server_options::get_program_options(int argc, char *argv[]) {
    ProgramOptions options = {
        .port = 0,
        .input_path = "",
        .timeout = 5
    };

    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-t") {
            if (i + 1 >= argc) {
                std::cout << "No value specified for timeout.\n";
                exit(1);
            }

            options.timeout = std::stoi(argv[i + 1]);
            i++;
        }

        if (arg == "-p") {
            if (i + 1 >= argc) {
                std::cout << "No value specified for port.\n";
                exit(1);
            }

            options.port = read_port(argv[i + 1]);
            ++i;
        } else if (arg == "-f") {
            if (i + 1 >= argc) {
                std::cout << "No value specified for game description file.\n";
                exit(1);
            }

            options.input_path = argv[i + 1];
            ++i;
        }
    }

    if (options.input_path.empty()) {
        std::cout << "Missing mandatory argument game description file.\n";
        exit(1);
    }

    return options;
}

