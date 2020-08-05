#pragma once
#include "network/network.h"
#include "input/input_driver.h"
#include "input/devices_descriptors.h"

namespace lap_rem::logic {

    class client {
    public:
        client();

        void start();
        void stop();

    private:
        input::input_driver input_driver;
        void on_input(input::input_driver::SharedEvent& sev);

    };
}



