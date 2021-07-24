#define _WIN32_WINNT 0x0A00

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "offsets.hpp"

#include <stdlib.h>

typedef void (*PACKET_CALLBACK)(unsigned short id, const char* data, unsigned int size);

class Connection {
public:
    Connection(std::string ip, unsigned short port);

    bool send_packet(unsigned short id, const void* data, unsigned int size);
    bool check_packets();

    void close();

    PACKET_CALLBACK on_packet = nullptr;

private:
    asio::io_context ctx;
    asio::ip::tcp::endpoint ep;
    asio::ip::tcp::socket socket;
    asio::error_code ec;

    std::vector<char> bytes_available;

    bool got_size;
    unsigned int packet_size;
};