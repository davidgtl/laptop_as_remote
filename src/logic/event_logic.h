#pragma once

#include <unordered_map>
#include "input/input_driver.h"
#include "input/devices_descriptors.h"

namespace laprem::logic {

    class event_logic {
    private:
        std::vector<input::input_driver::SharedEvent> event_queue;
    public:
        event_logic(std::unordered_map<u_int, input::device_descriptor>::iterator it);

    };

}
