#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>
#include <vector>

namespace lap_rem::network {

    class discovery {
    private:
        int port;
        std::string reply;
        std::thread* worker;
        void loop_listener();

    public:
        explicit discovery(int port);

        void start_listener(const std::string& reply);
        void search_listeners();

    };

}

