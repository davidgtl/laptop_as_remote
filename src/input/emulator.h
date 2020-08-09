#pragma once

#include <string>
#include "input/linux_structs.h"

namespace lap_rem::input {

    class emulator {
    public:
        enum device_preset {
            mouse,
            keyboard,
            touchpad,
            tablet
        };
        typedef struct device_info {
            char name[80];
            bus_types bus_type;
            uint16_t vendor;
            uint16_t product;
            uint16_t version;
            bool events[event_types::EV_CNT];
            bool key_bits[event_code::key::KEY_CNT];
            bool rel_bits[event_code::rel::REL_CNT];
            bool abs_bits[event_code::abs::ABS_CNT];
            bool msc_bits[event_code::msc::MSC_CNT];
            bool led_bits[event_code::led::LED_CNT];
            bool snd_bits[event_code::snd::SND_CNT];
            //bool ff_bits[event_code::keys::KEY_CNT];
            bool sw_bits[event_code::sw::SW_CNT];
            //bool prop_bits[event_code::keys::KEY_CNT];

        } device_info;

        emulator(device_preset p);

        emulator(device_info info);

        int start();

        int send_event(event_types type, uint16_t code, int32_t value) const;

        int send_event(input_event ev) const;

        int send_key(uint32_t key, bool down) const;

        int stop() const;
    private:
        int send_syn_report() const;

        int uinp_fd;

        int setup_keyboard();
        device_info devinfo;
    };
}



