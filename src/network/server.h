#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>
#include "util/array_view.h"
#include "util/callback.hpp"

namespace laprem::network {

    template<typename T>
    class Imessage_callback {
    private:
        void call(const char *data, int length) {
            on_message(array_view<T>((T *) data, length / sizeof(T)));
        }

    public:
        virtual void on_message(const array_view<T> &data) = 0;
    };


    template<typename T>
    class server {
    public:
        typedef struct { const array_view<T>& data; } on_message_args;

        server(int port, callback<void, on_message_args&> callback);

        void start();
    private:
        int port;
        bool is_running;

        void loop();

        std::thread *worker = nullptr;
        callback<void, on_message_args&> _callback;

    };
}

