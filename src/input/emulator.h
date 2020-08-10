#pragma once

#include <string>
#include "input/linux_structs.h"
#include "devices_descriptors.h"

namespace laprem::input {

    class emulator {
    public:
        enum device_preset {
            mouse,
            keyboard,
            touchpad,
            tablet
        };

        emulator();

        int start(device_preset p);
        int start(device_info info);

        int send_event(event_types type, uint16_t code, int32_t value) const;

        int send_event(input_event ev) const;

        int send_key(uint32_t key, bool down) const;

        int stop() const;
    private:
        int send_syn_report() const;

        int uinp_fd;
    };
}



