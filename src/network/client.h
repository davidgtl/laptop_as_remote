#pragma once
#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>

namespace lap_rem::network {

    class client {
    private:
        int port;

        void loop();

        void broadcasst_search_server();

        std::thread *worker;

    public:
        client(int port);

    };

}


