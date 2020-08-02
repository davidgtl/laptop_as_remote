#include "client.h"

namespace lap_rem::logic {

    client::client() : input_driver({*this, &client::on_input}) {

    }

    void client::start() {
        input_driver.start();
    }

    void client::on_input(input::input_driver::SharedEvent &sev) {

    }
};
