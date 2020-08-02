#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>
#include "util/array_view.h"
#include "util/callback.hpp"

namespace lap_rem::network {

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
    private:
        int port;

        void loop();

        std::thread *worker = nullptr;
        callback<void, const array_view<T>&> _callback;

    public:
        server(int port, callback<void, const array_view<T>&> callback);

        void start();
    };
}

