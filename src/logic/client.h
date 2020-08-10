#pragma once

#include "network/network.h"
#include "input/input_driver.h"
#include "input/emulator.h"
#include "input/devices_descriptors.h"
#include <mutex>
#include "util/queue.hpp"

namespace laprem::logic {

    class client {
    public:
        client();

        void start();

        void stop();

    private:
        input::input_driver input_driver;
        input::emulator emulator;
        queue<size_t> event_queue;
        std::mutex input_mutex;

        void on_input(input::input_driver::SharedEvent &sev, size_t position);

    };
}



