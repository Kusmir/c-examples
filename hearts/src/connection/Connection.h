#ifndef CONNECTION_H
#define CONNECTION_H

#include "Socket.h"
#include "PacketManager.h"

#include <vector>


class Connection {
public:
    Connection(Socket socket, IPEndpoint endpoint);
    Connection() : socket(-1, UNKNOWN) {};
    // ~Connection() { Close(); }
    void Close();
    void prepare_to_send(const char *data, size_t length);
    void prepare_to_send(const std::string &str);
    void prepare_to_send(const std::shared_ptr<Packet>& packet);
    size_t Send();
    size_t Recv();
    [[nodiscard]] bool has_message_to_send() const;
    [[nodiscard]] std::string hostname();
    [[nodiscard]] std::string peername();

    Socket socket;
    IPEndpoint endpoint;
    PacketManager input_manager;


private:
    static constexpr size_t BUFFER_SIZE = 256;
    std::vector<char> input_buffer;
    std::vector<char> output_buffer;

    std::string hostname_ = "";
    std::string peername_ = "";
};



#endif //CONNECTION_H
