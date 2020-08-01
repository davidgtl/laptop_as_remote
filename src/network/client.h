#pragma once
#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>

namespace lap_rem::network {

    class client {
    private:
        int port;

        boost::asio::ip::address addr;
        boost::asio::ip::tcp::socket *socket = nullptr;
        std::thread *worker = nullptr;

    public:
        client(const boost::asio::ip::address &addr, int port);

        void connect();

        void disconnect();

        template<typename T>
        void send(const T &data) {
            socket->send(boost::asio::buffer(data));
        }


    };

}


