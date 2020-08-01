#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace lap_rem::network {
    class server {
    private:
        int port;
        void loop();

    public:
        server(int port);

        void start();

    };
}

