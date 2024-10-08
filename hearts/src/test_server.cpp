#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread> // For std::thread
#include <atomic> // For std::atomic
#include <mutex>  // For std::mutex
#include <condition_variable> // For std::condition_variable

#include "common.h"
#include "exceptions.hpp"
#include "connection/Connection.h"

uint16_t read_port(char const *string) {
    char *endptr;
    errno = 0;
    unsigned long port = strtoul(string, &endptr, 10);
    if (errno != 0 || *endptr != 0 || port > UINT16_MAX) {
        exit(1);
        // fatal("%s is not a valid port number", string);
    }
    return (uint16_t) port;
}

IPEndpoint init_server_endpoint(uint16_t port) {
    sockaddr_in6 server_address = {};
    server_address.sin6_family = AF_INET6; // IPv6
    server_address.sin6_addr = in6addr_any; // Listening on all interfaces
    server_address.sin6_port = htons(port); // Port number

    return IPEndpoint(reinterpret_cast<sockaddr *>(&server_address));
}

// Function to receive messages in a separate thread
void receive_messages(Connection conn) {
    while (true) {
        // Receive messages
        try {
            conn.Recv();
            auto message = conn.input_manager.get_packet()->get_message();
            std::cout << conn.endpoint << ": " << message << std::endl;
        } catch (InvalidMessageException &e) {
            std::cerr << "Received invalid packet. Closing connection." << std::endl;
            conn.Close();
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        exit(1);

    uint16_t port = read_port(argv[1]);

    IPEndpoint server_endpoint = init_server_endpoint(port);
    Socket socket(IPv6);

    socket.Bind(server_endpoint);
    socket.set_option(IPV6_V6ONLY, false);
    socket.Listen();

    std::cout << "Listening on port: " << server_endpoint.port() << std::endl;

    IPEndpoint client_endpoint;
    Socket client_socket = socket.Accept(client_endpoint);
    std::cout << "Accepted connection from " << client_endpoint.ip_string() << std::endl;

    Connection conn(client_socket, client_endpoint);

    std::thread receiver_thread(receive_messages, conn);

    while (true) {
        std::string s;
        std::getline(std::cin, s);
        s += "\r\n";

        conn.prepare_to_send(s);

        size_t length = s.size();
        size_t total_sent = 0;
        while (total_sent < length) {
            size_t sent = conn.Send();
            std::cout << "sent: " << sent << std::endl;
            total_sent += sent;
        }
    }

    return 0;
}
