#include "emulator.h"
#include "linux_structs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

namespace lap_rem::input {

    namespace presets {
        using device_info = emulator::device_info;

        device_info keyboard() {
            device_info res = {
                    .name = "Laprem emulated keyboard",
                    .bus_type = (bus_types) (BUS_USB),
                    .vendor = 0x1,
                    .product = 0x1,
                    .version = 0x0,
                    .events = {[EV_SYN] = true, [EV_KEY] = true}
            };

            for (int i = 0; i < KEY_CNT; i++)
                res.key_bits[i] = true;

            return res;
        }

        emulator::device_info mouse() {
            device_info res = {
                    .name = "Laprem emulated mouse",
                    .bus_type = (bus_types) (BUS_USB),
                    .vendor = 0x1,
                    .product = 0x2,
                    .version = 0x0,
                    .events = {[EV_SYN] = true, [EV_KEY] = true, [EV_REL] = true}
            };

            for (int i = BTN_MISC; i <= BTN_GEAR_UP; i++)
                res.key_bits[i] = true;

            for (int i = REL_X; i <= REL_MISC; i++)
                res.rel_bits[i] = true;
        }
    }

    int emulator::start() {
        struct uinput_setup uinp;
        // Open the input device
        uinp_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (uinp_fd < 0) {

            printf("Unable to open /dev/uinput\n");
            return -1;
        }

        // Setup the uinput device
        for (int i = 0; i < sizeof(this->devinfo.events); i++)
            if (this->devinfo.events[i])
                if (ioctl(uinp_fd, UI_SET_EVBIT, i) < 0)
                    return -1;

        if (this->devinfo.events[EV_KEY])
            for (int i = 0; i < sizeof(this->devinfo.key_bits); i++)
                if (this->devinfo.key_bits[i])
                    if (ioctl(uinp_fd, UI_SET_KEYBIT, i) < 0)
                        return -1;

        if (this->devinfo.events[EV_REL])
            for (int i = 0; i < sizeof(this->devinfo.rel_bits); i++)
                if (this->devinfo.rel_bits[i])
                    if (ioctl(uinp_fd, UI_SET_RELBIT, i) < 0)
                        return -1;

        if (this->devinfo.events[EV_ABS])
            for (int i = 0; i < sizeof(this->devinfo.abs_bits); i++)
                if (this->devinfo.abs_bits[i])
                    if (ioctl(uinp_fd, UI_SET_ABSBIT, i) < 0)
                        return -1;

        if (this->devinfo.events[EV_MSC])
            for (int i = 0; i < sizeof(this->devinfo.msc_bits); i++)
                if (this->devinfo.msc_bits[i])
                    if (ioctl(uinp_fd, UI_SET_MSCBIT, i) < 0)
                        return -1;

        if (this->devinfo.events[EV_LED])
            for (int i = 0; i < sizeof(this->devinfo.led_bits); i++)
                if (this->devinfo.led_bits[i])
                    if (ioctl(uinp_fd, UI_SET_LEDBIT, i) < 0)
                        return -1;

        if (this->devinfo.events[EV_SND])
            for (int i = 0; i < sizeof(this->devinfo.snd_bits); i++)
                if (this->devinfo.snd_bits[i])
                    if (ioctl(uinp_fd, UI_SET_SNDBIT, i) < 0)
                        return -1;

        if (this->devinfo.events[EV_SW])
            for (int i = 0; i < sizeof(this->devinfo.sw_bits); i++)
                if (this->devinfo.sw_bits[i])
                    if (ioctl(uinp_fd, UI_SET_SWBIT, i) < 0)
                        return -1;


        memset(&uinp, 0, sizeof(uinp)); // Intialize the uInput device to NULL
        snprintf(uinp.name, UINPUT_MAX_NAME_SIZE, "%s", devinfo.name);
        uinp.id.bustype = devinfo.bus_type;
        uinp.id.vendor = devinfo.vendor;
        uinp.id.product = devinfo.product;
        uinp.id.version = devinfo.version;

        if (ioctl(uinp_fd, UI_DEV_SETUP, &uinp) < 0)
            printf("Unable to setup UINPUT device.");

        if (ioctl(uinp_fd, UI_DEV_CREATE) < 0) {
            printf("Unable to create UINPUT device.");
            return -1;
        }

        return 0;
    }

    int emulator::send_event(event_types type, uint16_t code, int32_t value) const {
        input::input_event event = {.type = type, .code = code, .value = value};
        gettimeofday(&event.time, nullptr);

        if (write(uinp_fd, &event, sizeof(event)) < 0)
            return -1;

        if (send_syn_report() < 0)
            return -1;

        return 0;
    }

    int emulator::send_syn_report() const {
        input::input_event event = {.type = EV_SYN, .code = SYN_REPORT, .value = 0};
        return write(uinp_fd, &event, sizeof(event));
    }

    int emulator::stop() const {
        if (ioctl(uinp_fd, UI_DEV_DESTROY) < 0)
            return -1;
        if (close(uinp_fd) < 0)
            return -1;
        return 0;
    }

    int emulator::send_key(uint32_t key, bool down) const {
        send_event((event_types) EV_KEY, key, down);
        return 0;
    }

    int emulator::send_event(input::input_event ev) const {
        if (write(uinp_fd, &ev, sizeof(ev)) < 0)
            return -1;

        if (send_syn_report() < 0)
            return -1;

        return 0;
    }

    emulator::emulator(emulator::device_preset p) {
        switch (p) {
            case keyboard:
                this->devinfo = presets::keyboard();
                break;
            case mouse:
                this->devinfo = presets::mouse();
                break;
            default:
                this->devinfo = {};
        }
    }

    emulator::emulator(emulator::device_info devinfo) {
        this->devinfo = devinfo;
    }

    int emulator::setup_keyboard() {
        return 0;
    }

}
