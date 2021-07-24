#include "connection.hpp"

Connection::Connection(std::string ip, unsigned short port)
        : ep(asio::ip::make_address(ip, ec), port), socket(ctx) {
    socket.connect(ep, ec);

    if (ec)
        MessageBoxA(NULL, ec.message().c_str(), "Could not connect!", MB_ICONINFORMATION);
}

bool Connection::send_packet(unsigned short id, const void* data, unsigned int size) {
    char* send_data = new char[size + sizeof(int) + sizeof(short)];

    *(int*)send_data = size + sizeof(short);
    *(short*)(send_data + sizeof(int)) = id;

    memcpy(send_data + sizeof(int) + sizeof(short), data, size);

    socket.write_some(asio::buffer(send_data, size + sizeof(int) + sizeof(short)), ec);
    return (bool)!ec;
}

void Connection::close() {
    socket.close(ec);
}

bool Connection::check_packets() {
    size_t bytes = socket.available();

    if (bytes > 0) {
        std::vector<char> rdata(bytes);
        socket.read_some(asio::buffer(rdata.data(), rdata.size()), ec);

        if (ec) return false;

        bytes_available.insert(
            bytes_available.end(),
            rdata.begin(),
            rdata.end()
        );
    }

    if (!got_size && bytes_available.size() >= 4) {
        packet_size = *(unsigned int*)bytes_available.data();
        got_size = true;

        bytes_available.erase(bytes_available.begin(), bytes_available.begin() + 4);
    }

    if (got_size && bytes_available.size() >= packet_size) {
        char* packet_data = new char[packet_size];

        memcpy(packet_data, bytes_available.data(), packet_size);
        bytes_available.erase(bytes_available.begin(), bytes_available.begin() + packet_size);

        got_size = false;

        unsigned short id = *(unsigned short*)packet_data;

        on_packet(id, packet_data + sizeof(short), packet_size - sizeof(short));
    }
}