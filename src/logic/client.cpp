#include "client.h"
#include "input/devices_descriptors.h"

namespace lap_rem::logic {
    using namespace input;

    client::client() : input_driver({*this, &client::on_input}) {

    }

    void client::start() {
        devices::query();
        devices::loadwatched();

        input_driver.start();


    }

    void client::on_input(input::input_driver::SharedEvent &sev) {
        devices::iswatched(sev.device_identifier);
    }

    void client::stop() {
        input::devices::savewatched();
    }
};
