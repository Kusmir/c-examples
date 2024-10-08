#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <sys/poll.h>

#include "exceptions.hpp"
#include "connection/Connection.h"
#include "connection/IPEndpoint.h"
#include "connection/Socket.h"
#include "connection/packets/PacketsInclude.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/Seat.h"
#include "utils/server_options.h"

using namespace game_engine;

int TIMEOUT;
uint16_t port;
std::string input_path;

constexpr size_t MAX_CLIENTS = 10;
pollfd poll_descriptors[MAX_CLIENTS];
std::array<std::vector<Card>, 4> starting_hands;
std::vector<std::shared_ptr<Packet> > game_history;
IPEndpoint server_endpoint;
GameEngine game;

struct Client {
    Seat seat = Seat::NONE;
    Connection connection;
    int timeout = TIMEOUT;
};

std::array<Client, MAX_CLIENTS> clients;

size_t get_player_id(Seat seat) {
    for (size_t i = 1; i < MAX_CLIENTS; i++)
        if (clients[i].seat == seat) return i;
    return MAX_CLIENTS;
}

void disconnect_client(size_t client_id) {
    Client &client = clients[client_id];
    if (client.seat != NONE)
        game.remove_player(client.seat);

    client.connection.Close();
    poll_descriptors[client_id].fd = -1;
};

bool waiting_for_response(size_t client_id) {
    const Client &client = clients[client_id];

    if (poll_descriptors[client_id].fd == -1) return false;
    if (client.connection.has_message_to_send()) return false;
    if (client.seat == NONE) return true; // waiting for IAM
    return (!game.paused() and game.active_player_seat() == client.seat);
};

void start_deal() {
    game_history.clear();
    auto hands = game.start_new_deal();
    if (hands[0].empty()) {
        // Game finished
        auto scores = game.get_total_scores();
        auto scores_packet = std::make_shared<Total>(scores);

        for (Seat seat: {N, E, S, W}) {
            size_t id = get_player_id(seat);
            clients[id].connection.prepare_to_send(scores_packet);
            report_packet(clients[id].connection, scores_packet, true);
        }
        return;
    }

    Seat starting_seat = game.active_player_seat();
    size_t trick_id = game.current_trick_id();

    starting_hands = hands;
    for (Seat seat: {N, E, S, W}) {
        size_t id = get_player_id(seat);
        auto deal_packet = std::make_shared<Deal>(
            trick_id, starting_seat, hands[seat]);
        clients[id].connection.prepare_to_send(deal_packet);
        report_packet(clients[id].connection, deal_packet, true);
    }

    size_t starting_id = get_player_id(starting_seat);
    Client &starting_player = clients[starting_id];
    auto trick_packet = std::make_shared<TrickPacket>(
        trick_id, std::vector<Card>{});
    starting_player.connection.prepare_to_send(trick_packet);
    report_packet(starting_player.connection, trick_packet, true);
}

void end_deal() {
    auto scores = game.end_deal();
    auto scores_packet = std::make_shared<Score>(scores);
    for (Seat seat: {N, E, S, W}) {
        size_t id = get_player_id(seat);
        clients[id].connection.prepare_to_send(scores_packet);
        report_packet(clients[id].connection, scores_packet, true);
    }

    start_deal();
}

void next_trick();
void end_trick() {
    size_t trick_id = game.current_trick_id();
    std::vector<Card> cards = game.current_trick().cards;
    Seat winner_seat = game.determine_winner();

    auto taken_packet = std::make_shared<Taken>(trick_id, cards, winner_seat);
    game_history.emplace_back(taken_packet);

    for (Seat seat: {N, E, S, W}) {
        size_t id = get_player_id(seat);
        clients[id].connection.prepare_to_send(taken_packet);
        report_packet(clients[id].connection, taken_packet, true);
    }

    if (trick_id >= GameEngine::MAX_TRICKS)
        end_deal();
    else
        next_trick();
}

void next_trick() {
    if (starting_hands[0].empty() and game_history.empty()) {
        start_deal();
        return;
    }

    if (game.current_trick().cards.size() == 4) {
        end_trick();
        return;
    }

    Client &active_player =
            clients[get_player_id(game.active_player_seat())];

    size_t trick_id = game.current_trick_id();
    auto cards = game.current_trick().cards;
    auto trick_packet = std::make_shared<TrickPacket>(trick_id, cards);

    active_player.connection.prepare_to_send(trick_packet);
    report_packet(active_player.connection, trick_packet, true);
};

void process(size_t client_id, TakePlace &packet) {
    Client &client = clients[client_id];

    // Second IAM message -> disconnect
    if (client.seat != NONE) {
        disconnect_client(client_id);
        return;
    }

    Seat want_seat = Seat(packet.seat());
    if (game.add_player(want_seat)) {
        client.seat = want_seat;
        std::clog << "Client has taken seat " << client.seat << std::endl;
        if (!starting_hands[0].empty()) {
            auto deal_packet = std::make_shared<Deal>(
                game.rules(),
                game.first_seat(),
                starting_hands[client.seat]
            );

            client.connection.prepare_to_send(deal_packet);
            report_packet(client.connection, deal_packet, true);

            for (const auto &history_packet: game_history) {
                client.connection.prepare_to_send(history_packet);
                report_packet(client.connection, history_packet, true);
            }
        }

        // Resume game if possible
        if (!game.paused()) {
            next_trick();
        }
    } else {
        // The seat client requested is already taken.
        std::clog << "Client has requested a seat that is already taken: "
                << packet.seat() << std::endl;
        auto busy_packet = std::make_shared<Busy>(game.taken_seats());
        client.connection.prepare_to_send(busy_packet);
        report_packet(client.connection, busy_packet, true);
    }
}

void process(size_t client_id, const TrickPacket &packet) {
    if (game.paused()) return;

    Client &client = clients[client_id];
    auto send_wrong = [&]() {
        auto wrong = std::make_shared<Wrong>(game.current_trick_id());
        auto trick_packet = std::make_shared<TrickPacket>(
            game.current_trick().id, game.current_trick().cards
        );

        client.connection.prepare_to_send(wrong);
        report_packet(client.connection, wrong, true);
        client.connection.prepare_to_send(trick_packet);
        report_packet(client.connection, trick_packet, true);
    };

    if (game.finished() or client.seat == NONE
        or client.seat != game.active_player_seat()) {
        disconnect_client(client_id);
        return;
    }

    if (packet.trick_id() != game.current_trick_id()
        or packet.cards().size() != 1) {
        send_wrong();
        return;
    }

    if (game.place_card(client.seat, packet.cards()[0])) {
        next_trick();
    } else {
        send_wrong();
    }
}

void process(size_t client_id) {
    Client &client = clients[client_id];
    Connection &conn = client.connection;
    assert(conn.input_manager.has_pending_packets());
    // Reset client timeout
    client.timeout = TIMEOUT;

    while (conn.input_manager.has_pending_packets()) {
        auto packet = conn.input_manager.get_packet();
        report_packet(conn, packet, false);
        switch (packet->type()) {
            case IAM: {
                auto iam = *std::static_pointer_cast<TakePlace>(packet);
                process(client_id, iam);
                break;
            }
            case TRICK: {
                auto trick = *std::static_pointer_cast<TrickPacket>(packet);
                process(client_id, trick);
                break;
            }
            default:
                //Invalid packet
                disconnect_client(client_id);
        }
    }
}

IPEndpoint init_server_endpoint(uint16_t port) {
    sockaddr_in6 server_address = {};
    server_address.sin6_family = AF_INET6; // IPv6
    server_address.sin6_addr = in6addr_any; // Listening on all interfaces
    server_address.sin6_port = htons(port); // Port number

    return IPEndpoint(reinterpret_cast<sockaddr *>(&server_address));
}

IPEndpoint get_real_endpoint(int socket_fd) {
    sockaddr_in6 server_address = {};
    socklen_t length = (socklen_t) sizeof server_address;
    if (getsockname(socket_fd, (struct sockaddr *) &server_address, &length) < 0) {
        throw SysFailed("getsockname");
    }

    return IPEndpoint(reinterpret_cast<sockaddr *>(&server_address));
}

size_t find_space(pollfd poll_descriptors[], size_t size) {
    for (size_t i = 0; i < size; i++)
        if (poll_descriptors[i].fd == -1) return i;
    return size;
}

void run(int argc, char *argv[]) {
    auto options = server_options::get_program_options(argc, argv);

    TIMEOUT = options.timeout * 1000;
    port = options.port;
    input_path = options.input_path;

    server_endpoint = init_server_endpoint(port);
    game = GameEngine(input_path);
    game_history = {};
    starting_hands = {};

    Socket main_socket(IPv6);
    main_socket.set_option(IPV6_V6ONLY, false);
    main_socket.set_option(SO_REUSEADDR, true);
    main_socket.Bind(server_endpoint);
    main_socket.Listen();

    try {
        server_endpoint = get_real_endpoint(main_socket.fd());
    } catch (SysFailed &e) {
        std::cerr << e.what();
        exit(1);
    }

    std::clog << "Listening on port " << server_endpoint.port() << std::endl;

    poll_descriptors[0].fd = main_socket.fd();
    for (size_t i = 1; i < MAX_CLIENTS; i++)
        poll_descriptors[i].fd = -1;


    do {
        // Reset poll_descriptors events
        bool has_something_to_write = false;
        for (size_t i = 0; i < MAX_CLIENTS; ++i) {
            poll_descriptors[i].revents = 0;
            poll_descriptors[i].events = 0;
            if (poll_descriptors[i].fd != -1) {
                poll_descriptors[i].events |= POLLIN;
                if (clients[i].connection.has_message_to_send()) {
                    has_something_to_write = true;
                    poll_descriptors[i].events |= POLLOUT;
                }
            }
        }

        if (game.finished() and !has_something_to_write) {
            break;
        }

        // Set timeout to minimal among connected clients
        int timeout = -1;
        for (size_t i = 1; i < MAX_CLIENTS; ++i) {
            if (poll_descriptors[i].fd == -1) continue;

            if (waiting_for_response(i)) {
                if (timeout == -1)
                    timeout = clients[i].timeout;
                else
                    timeout = std::min(timeout, clients[i].timeout);
            }
        }

        // Execute poll and measure waiting time.
        auto start_time = std::chrono::system_clock::now();
        int poll_status = poll(poll_descriptors, MAX_CLIENTS, timeout);
        if (poll_status == -1)
            throw SysFailed("poll");
        auto end_time = std::chrono::system_clock::now();

        // Update client timeouts.
        int time_passed = static_cast<int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                end_time - start_time
            ).count());

        for (size_t i = 1; i < MAX_CLIENTS; i++) {
            if (poll_descriptors[i].fd == -1 or !waiting_for_response(i))
                continue;

            if (poll_status > 0 and poll_descriptors[i].revents & POLLIN)
                clients[i].timeout = TIMEOUT;
            else
                clients[i].timeout -= time_passed;

            if (clients[i].timeout < 0) {
                std::clog << "Connection " << i << " timed out." << std::endl;
                if (clients[i].seat != NONE and
                    game.active_player_seat() == clients[i].seat) {
                    std::clog << "Retransmitting TRICK message." << std::endl;
                    auto trick_packet = std::make_shared<TrickPacket>(
                        game.current_trick_id(),
                        game.current_trick().cards
                    );
                    clients[i].connection.prepare_to_send(trick_packet);
                    report_packet(clients[i].connection, trick_packet, true);
                    clients[i].timeout = TIMEOUT;
                } else
                    disconnect_client(i);
            }
        }

        if (poll_status == 0) {
            //TODO: idk do something
            continue;
        }

        // Accept new connection
        if (poll_descriptors[0].revents & POLLIN) {
            // New connection.
            IPEndpoint new_client_endpoint;
            Socket new_client_socket = main_socket.Accept(new_client_endpoint);
            new_client_socket.set_blocking(false);

            Connection conn(new_client_socket, new_client_endpoint);

            size_t id = find_space(poll_descriptors, MAX_CLIENTS);
            if (id >= MAX_CLIENTS) {
                conn.Close();
                std::clog << "Too many clients." << std::endl;
            } else {
                std::clog << "Accepted new connection (" << id << ")"
                        << " from " << new_client_endpoint << std::endl;
                poll_descriptors[id].fd = new_client_socket.fd();
                clients[id] = Client{
                    .seat = NONE,
                    .connection = conn,
                    .timeout = TIMEOUT
                };

                if (!game.paused()) {
                    std::clog << "Game already in progress." << std::endl;
                    auto busy = std::make_shared<Busy>(std::vector<Seat>{
                        N, E, S, W
                    });
                    clients[id].connection.prepare_to_send(busy);
                    report_packet(clients[id].connection, busy, true);
                }
            }
        }

        // serve clients
        for (size_t i = 1; i < MAX_CLIENTS; ++i) {
            if (poll_descriptors[i].fd == -1) continue;

            Client &client = clients[i];
            if (poll_descriptors[i].revents & (POLLIN | POLLERR)) {
                size_t received;
                try {
                    received = client.connection.Recv();
                } catch (SysFailed &e) {
                    std::cerr << "error when reading message from "
                            << "connection " << i << "(errno " << errno
                            << ", " << std::strerror(errno) << ")\n";
                    disconnect_client(i);
                    continue;
                }

                if (received == 0) {
                    std::cerr << "connection closed by client (" << i << ")" <<
                            std::endl;
                    disconnect_client(i);
                    continue;
                }

                if (client.connection.input_manager.has_pending_packets())
                    process(i);
            }

            if (poll_descriptors[i].revents & POLLOUT) {
                size_t sent;
                try {
                    sent = client.connection.Send();
                } catch (SysFailed &e) {
                    std::cerr << "error when sending message to "
                            << "connection " << i << "(errno " << errno
                            << ", " << std::strerror(errno) << ")\n";
                    disconnect_client(i);
                    continue;
                }

                assert(sent > 0);
            }
        }
    } while (true);

    for (size_t i = 1; i < MAX_CLIENTS; i++)
        if (poll_descriptors[i].fd != -1)
            clients[i].connection.Close();
    main_socket.Close();
}

int main(int argc, char *argv[]) {
    try {
        run(argc, argv);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
