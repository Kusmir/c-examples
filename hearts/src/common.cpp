#include "common.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string current_timestamp_string() {
    auto now = std::chrono::system_clock::now();

    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm now_tm = {};
    localtime_r(&now_time_t, &now_tm);

    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) % 1000;

    std::ostringstream timestamp;
    timestamp << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%S") << '.'
            << std::setfill('0') << std::setw(3) << now_ms.count();

    return timestamp.str();
}

void report_packet(
    Connection &conn, const std::shared_ptr<Packet> &packet, bool sent) {
    std::string message = packet->get_message();
    if (packet->type() == INVALID_PACKET) message += "\r\n";

    std::string sender = (sent ? conn.hostname() : conn.peername());
    std::string receiver = (sent ? conn.peername() : conn.hostname());
    std::cout << "[" << sender << "," << receiver << ","
            << current_timestamp_string() << "] " << packet->get_message();
}
