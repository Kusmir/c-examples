#include <sys/socket.h>
#include <iostream>

#include "client_options.h"

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

client_options::ProgramOptions
client_options::get_program_options(int argc, char *argv[]) {
    ProgramOptions options = {
        .host = "",
        .port = 0,
        .ip_version = UNKNOWN,
        .seat = NONE,
        .automatic = false
    };

    bool got_port = false;
    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-N") options.seat = N;
        if (arg == "-E") options.seat = E;
        if (arg == "-S") options.seat = S;
        if (arg == "-W") options.seat = W;
        if (arg == "-4") options.ip_version = IPv4;
        if (arg == "-6") options.ip_version = IPv6;
        if (arg == "-a") options.automatic = true;
        if (arg == "-p") {
            if (i + 1 >= argc) {
                std::cout << "No value specified for port.\n";
                exit(1);
            }

            options.port = read_port(argv[i + 1]);
            got_port = true;
            ++i;
        } else if (arg == "-h") {
            if (i + 1 >= argc) {
                std::cout << "No value specified for port.\n";
                exit(1);
            }

            options.host = argv[i + 1];
            ++i;
        }
    }

    if (options.host == "") {
        std::cout << "Missing mandatory argument host.\n";
        exit(1);
    }

    if (!got_port) {
        std::cout << "Missing mandatory argument port.\n";
        exit(1);
    }

    if (options.seat == NONE) {
        std::cout << "Missing mandatory argument seat.\n";
        exit(1);
    }

    return options;
}

void client_options::log_program_options(
    const client_options::ProgramOptions &options) {
    std::clog << "Host: " << options.host << "\n";
    std::clog << "Port: " << options.port << "\n";
    std::clog << "IP Version: " << (
        options.ip_version == IPv4
            ? "IPv4"
            : (options.ip_version == IPv6 ? "IPv6" : "Unspecified")
    ) << "\n";
    std::clog << "Position: " << seat_to_char(options.seat) << "\n";
    std::clog << "Automatic: "
            << (options.automatic ? "Yes" : "No") << '\n';
}
