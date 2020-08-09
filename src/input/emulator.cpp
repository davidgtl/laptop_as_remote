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

    int emulator::start() {
        struct uinput_setup uinp;
        // Open the input device
        uinp_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (uinp_fd < 0) {

            printf("Unable to open /dev/uinput\n");
            return -1;
        }

        // Setup the uinput device
        if (ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY) < 0)
            return -1;
        /*if (ioctl(uinp_fd, UI_SET_EVBIT, EV_REL) < 0)
            printf("unable to write");
        if (ioctl(uinp_fd, UI_SET_RELBIT, REL_X) < 0)
            printf("unable to write");
        if (ioctl(uinp_fd, UI_SET_RELBIT, REL_Y) < 0)
            printf("unable to write");*/
        for (int i = 1; i < 248; i++) {
            if(ioctl(uinp_fd, UI_SET_KEYBIT, i) < 0)
                return -1;
        }
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_TOUCH);
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_LEFT);
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MIDDLE);
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_RIGHT);
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_FORWARD);
        //ioctl(uinp_fd, UI_SET_KEYBIT, BTN_BACK);


        memset(&uinp, 0, sizeof(uinp)); // Intialize the uInput device to NULL
        snprintf(uinp.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
        uinp.id.bustype = BUS_USB;
        uinp.id.vendor = 0x1;
        uinp.id.product = 0x1;
        uinp.id.version = 1;


        if (ioctl(uinp_fd, UI_DEV_SETUP, &uinp) < 0)
            printf("Unable to setup UINPUT device.");

        if (ioctl(uinp_fd, UI_DEV_CREATE) < 0) {
            printf("Unable to create UINPUT device.");
            return -1;
        }

        return 1;
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
        send_event((event_types)EV_KEY, key, down);
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


        }
    }

    emulator::emulator(emulator::device_info) {

    }

    int emulator::setup_keyboard() {
        return 0;
    }

}
