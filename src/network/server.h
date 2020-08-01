#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>

namespace lap_rem::network {

    class Imessage_callback {
    public:
        virtual void on_message(char* message, int size) = 0;
    };

    class server {
    private:
        int port;

        void loop();

        std::thread *worker;
        Imessage_callback *callback;

    public:
        server(int port, Imessage_callback &mc);

        void start();
    };
}

