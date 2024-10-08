#ifndef CLIENT_OPTIONS_H
#define CLIENT_OPTIONS_H

#include "../connection/IPVersion.h"
#include "../common.h"

namespace client_options {
    struct ProgramOptions {
        std::string host;
        uint16_t port;
        IPVersion ip_version;
        Seat seat;
        bool automatic;
    };

    ProgramOptions get_program_options(int argc, char* argv[]);
    void log_program_options(const ProgramOptions& options);
}

#endif //CLIENT_OPTIONS_H
