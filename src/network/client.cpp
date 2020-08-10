#include "client.h"

namespace laprem::network {

    client::client(const boost::asio::ip::address& addr, int port) {
        this->addr = addr;
        this->port = port;
    }

    void client::connect() {
        boost::asio::io_service ios;
        boost::asio::ip::tcp::endpoint endpoint(addr, port);
        socket = new boost::asio::ip::tcp::socket(ios);
        socket->connect(endpoint);
    }

    void client::disconnect() {
        socket->close();
    }

}
