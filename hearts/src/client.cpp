#include <atomic>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <vector>
#include <poll.h>

#include "common.h"

#include "connection/Connection.h"
#include "connection/packets/Busy.hpp"
#include "connection/packets/Deal.hpp"
#include "utils/client_options.h"
#include "connection/packets/Packet.h"
#include "connection/packets/Score.hpp"
#include "connection/packets/Taken.hpp"
#include "connection/packets/TakePlace.hpp"
#include "connection/packets/Total.hpp"
#include "connection/packets/Wrong.hpp"
#include "connection/packets/TrickPacket.hpp"

#include "exceptions.hpp"
#include "utils/parser.h"

using game_engine::Card;

std::vector<std::vector<game_engine::Card> > tricks;
std::vector<Card> cards_in_hand;

// Waiting for user input of card choice.
bool should_place_card = false;
bool automatic_player = false;
bool reporting = false;
Seat seat;
size_t current_trick_id = 1;
size_t deal_id = 0;
size_t scores_received = 0;
bool received_total = false;

bool remove_card_from_hand(const Card &card) {
    auto it =
            std::find(cards_in_hand.begin(), cards_in_hand.end(), card);
    if (it != cards_in_hand.end()) {
        cards_in_hand.erase(it);
        return true;
    }

    return false;
}

Card choose_card(const std::vector<Card> current_trick) {
    assert(!cards_in_hand.empty());

    if (!current_trick.empty()) {
        // Choose a card to color
        Card::Color target_color = current_trick[0].color();
        for (auto it = cards_in_hand.begin(); it != cards_in_hand.end(); ++it) {
            if (it->color() == target_color) {
                Card chosen_card = *it;
                cards_in_hand.erase(it);
                return chosen_card;
            }
        }
    }

    // If we don't have a card with matching color or we're the first
    // one to play, we place any card from our hand.
    Card chosen_card = cards_in_hand.back();
    cards_in_hand.pop_back();
    return chosen_card;
}

// TODO: better processing for automatic player
void process_packet(const std::shared_ptr<Packet> &packet,
                    Connection &connection) {
    // if (reporting)
    //     std::cout << connection.endpoint << " "
    //             << packet->get_message();

    should_place_card = false;
    switch (packet->type()) {
        case DEAL: {
            auto deal = std::static_pointer_cast<Deal>(packet);
            if (automatic_player) report_packet(connection, deal, false);
            // Ignore invalid deal
            if (!cards_in_hand.empty()) break;

            deal_id++;
            cards_in_hand = deal->cards();
            tricks.clear();
            current_trick_id = 1;
            if (!automatic_player) {
                char first_pos = seat_to_char(deal->first_player_position());
                std::cout << "New deal " << deal->ruleset() << ": " <<
                        "staring place: " << first_pos << ", " <<
                        "your cards: " << cards_in_hand << '\n';
            }
            break;
        }
        case TRICK: {
            auto trick = std::static_pointer_cast<TrickPacket>(packet);
            if (automatic_player) report_packet(connection, trick, false);
            // Ignore invalid message
            if (trick->trick_id() != current_trick_id)
                break;

            if (automatic_player) {
                Card chosen_card = choose_card(trick->cards());
                auto trick_packet = std::make_shared<TrickPacket>(
                    trick->trick_id(), chosen_card);
                connection.prepare_to_send(trick_packet);
                report_packet(connection, trick_packet, true);
            } else {
                should_place_card = true;
                std::cout << "Trick: (" << trick->trick_id() << ") " <<
                        trick->cards() << '\n' <<
                        "Available: " << cards_in_hand << '\n';
            }
            break;
        }
        case WRONG: {
            auto wrong = std::static_pointer_cast<Wrong>(packet);
            if (!automatic_player)
                std::cout << "Wrong message received in trick " <<
                        wrong->trick_id() << '\n';
            else
                report_packet(connection, wrong, false);

            break;
        }
        case BUSY: {
            auto busy = std::static_pointer_cast<Busy>(packet);
            // TODO: detect invalid BUSY
            if (!automatic_player) {
                std::cout << "Place busy, list of busy places received: " <<
                        busy->taken_seats() << '\n';
            } else {
                report_packet(connection, busy, false);
            }
            exit(1);
        }
        case SCORE: {
            auto score = std::static_pointer_cast<Score>(packet);
            if (automatic_player)
                report_packet(connection, score, false);
            if (!cards_in_hand.empty())
                break;
            if (!automatic_player) {
                std::cout << "The scores are:\n";
                for (int seat = 0; seat < (int)SEATS; ++seat) {
                    std::cout << seat_to_char(seat) << " | " <<
                            score->scores()[seat] << '\n';
                }
            }
            scores_received++;
            break;
        }
        case TAKEN: {
            auto taken = std::static_pointer_cast<Taken>(packet);
            if (!automatic_player) {
                std::cout << "A trick " << taken->trick_id() << " is taken by "
                        << taken->active_seat() << ", cards " << taken->cards()
                        << "." << std::endl;
            } else {
                report_packet(connection, taken, false);
            }

            // Ignore invalid trick packet
            if (taken->trick_id() != current_trick_id)
                break;

            if (taken->active_seat() == seat)
                tricks.emplace_back(taken->cards());

            // TAKEN may be sent as recap of a game we joined in the middle.
            // In such case, we must adjust cards in hand and remove the cards
            // player before as played.
            for (const Card &card: taken->cards()) {
                remove_card_from_hand(card);
            }
            ++current_trick_id;

            break;
        }
        case TOTAL: {
            auto total = std::static_pointer_cast<Total>(packet);
            if (!automatic_player) {
                std::cout << "The total scores are:\n";
                for (int seat = 0; seat < (int) SEATS; ++seat) {
                    std::cout << seat_to_char(seat) << " | " <<
                            total->scores()[seat] << '\n';
                }
            } else {
                report_packet(connection, total, false);
            }
            received_total = true;
            break;
        }
        default:
            throw InvalidMessageException(packet->get_message());
    }
}

void process_command(const std::string &command, Connection &connection) {
    if (command == "cards") {
        std::cout << cards_in_hand << '\n';
        return;
    }

    if (command == "tricks") {
        std::cout << tricks << '\n';
        return;
    }

    Card card = parser::parse_player_command(command);
    if (should_place_card) {
        if (card.value() == Card::Value::INVALID_VALUE) {
            std::cerr << "Invalid command." << std::endl;
            return;
        }

        auto trick_packet = std::make_shared<TrickPacket>(
            current_trick_id, card);
        connection.prepare_to_send(trick_packet);
        should_place_card = false;
        if (automatic_player)
            report_packet(connection, trick_packet, true);
    } else {
        std::cerr << "Invalid command. It is not your turn." << std::endl;
    }
}

int run(int argc, char *argv[]) {
    auto options = client_options::get_program_options(argc, argv);

    client_options::log_program_options(options);
    automatic_player = options.automatic;
    reporting = options.automatic;
    seat = options.seat;

    IPEndpoint server_endpoint(
        options.host.c_str(),
        options.port,
        options.ip_version
    );

    Socket socket(server_endpoint.ip_version());

    Connection connection(socket, server_endpoint);

    std::cerr << "Connecting to server, please wait...\n";
    socket.Connect(server_endpoint);

    std::cerr << "Connected successfully! Server address: "
            << server_endpoint.ip_string() << "\n";

    auto iam = std::make_shared<TakePlace>(options.seat);
    connection.prepare_to_send(iam);
    if (automatic_player)
        report_packet(connection, iam, true);

    socket.set_blocking(false);

    constexpr size_t server_poll = 0;
    constexpr size_t cli_poll = 1;
    constexpr size_t connections_cnt = 2;

    pollfd poll_descriptors[connections_cnt];
    poll_descriptors[server_poll].fd = socket.fd();
    poll_descriptors[cli_poll].fd = (options.automatic ? -1 : STDIN);

    while (true) {
        for (auto &pd: poll_descriptors) {
            pd.revents = 0;
            pd.events = POLLIN;
        }

        if (connection.has_message_to_send()) {
            poll_descriptors[server_poll].events |= POLLOUT;
        }

        int poll_status = poll(poll_descriptors, 2, -1);
        if (poll_status < 0)
            throw SysFailed("poll");

        if (poll_descriptors[server_poll].revents & POLLOUT) {
            connection.Send();
        }

        if (poll_descriptors[server_poll].revents & POLLIN) {
            try {
                ssize_t received = connection.Recv();
                if (received == 0) {
                    if (scores_received == deal_id and received_total) {
                        std::clog << "Game finished.\n";
                        return 0;
                    }

                    std::cerr << "Connection closed by server.\n";
                    return 1;
                }

                while (connection.input_manager.has_pending_packets()) {
                    auto packet = connection.input_manager.get_packet();
                    process_packet(packet, connection);
                }
            } catch (InvalidMessageException &e) {
                std::cerr << "Received invalid message from server: " <<
                        e.message() << "\n";
            }
        }

        if (poll_descriptors[cli_poll].revents & POLLIN) {
            std::string command;
            std::getline(std::cin, command);
            try {
                process_command(command, connection);
            } catch (InvalidMessageException &e) {
                std::cerr << "Invalid command. Try again.\n";
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    try {
        return run(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
