#include "Connection.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>
#include <utility>


#include "../exceptions.hpp"

Connection::Connection(Socket socket, IPEndpoint endpoint)
    : socket(socket), endpoint(std::move(endpoint)) {
    output_buffer = std::vector<char>();
    input_buffer = std::vector<char>(BUFFER_SIZE);
}

void Connection::Close() {
    socket.Close();
}

void Connection::prepare_to_send(const char *data, size_t length) {
    output_buffer.insert(output_buffer.end(), data, data + length);
}

void Connection::prepare_to_send(const std::string &str) {
    prepare_to_send(str.c_str(), str.length());
}

void Connection::prepare_to_send(const std::shared_ptr<Packet> &packet) {
    std::string message = packet->get_message();
    prepare_to_send(message.c_str(), message.length());
}

//TODO: error handling
size_t Connection::Send() {
    size_t length = output_buffer.size();
    ssize_t sent = socket.Send(output_buffer.data(), length);
    if (sent > 0) {
        output_buffer.erase(
            output_buffer.begin(),
            output_buffer.begin() + sent);
    }

    return sent;
}

size_t Connection::Recv() {
    input_buffer = std::vector<char>(BUFFER_SIZE, 0);
    size_t length = socket.Recv(input_buffer.data(), input_buffer.size());
    if (length == 0) return 0;

    input_manager.process(input_buffer.data(), length);
    return length;
}

bool Connection::has_message_to_send() const {
    return !output_buffer.empty();
}

std::string Connection::hostname() {
    if (!hostname_.empty()) return hostname_;

   if (endpoint.ip_version() == IPv4) {
       sockaddr_in addr;
       socklen_t addr_len = sizeof(addr);
       if (getsockname(socket.fd(), (sockaddr*)&addr, &addr_len) == -1)
           throw SysFailed("getsockname");
       char ip[INET_ADDRSTRLEN];
       inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
       std::ostringstream oss;
       oss << ip << ":" << ntohs(addr.sin_port);
       return hostname_ = oss.str();
   } else {
       sockaddr_in6 addr;
       socklen_t addr_len = sizeof(addr);
       if (getsockname(socket.fd(), (sockaddr*)&addr, &addr_len) == -1)
           throw SysFailed("getsockname");

       char ip[INET6_ADDRSTRLEN];
       inet_ntop(AF_INET6, &addr.sin6_addr, ip, sizeof(ip));
       std::ostringstream oss;
       oss << ip << ":" << ntohs(addr.sin6_port);
       return hostname_ = oss.str();
   }
}

std::string Connection::peername() {
    if (!peername_.empty()) return peername_;

    if (socket.ip_version() == IPv4) {
        sockaddr_in addr{};
        socklen_t addr_len = sizeof(addr);
        if (getpeername(socket.fd(), (sockaddr*)&addr, &addr_len) == -1) {
           throw SysFailed("getsockname");
        }
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
        std::ostringstream oss;
        oss << ip << ":" << ntohs(addr.sin_port);
        return peername_ = oss.str();
    } else {
       sockaddr_in6 addr{};
       socklen_t addr_len = sizeof(addr);
       if (getpeername(socket.fd(), (sockaddr*)&addr, &addr_len) == -1)
           throw SysFailed("getsockname");

       char ip[INET6_ADDRSTRLEN];
       inet_ntop(AF_INET6, &addr.sin6_addr, ip, sizeof(ip));
       std::ostringstream oss;
       oss << ip << ":" << ntohs(addr.sin6_port);
       return peername_ = oss.str();
    }
}
