#pragma once

#include <semaphore.h>
#include <thread>
#include <string>
#include <vector>
#include "util/callback.hpp"
#include "linux_structs.h"

namespace laprem::input {

    class input_driver {

    public:

        typedef struct {
            int device_identifier;
            u_char delegated;
            input_event ev;
        } SharedEvent;

        input_driver(callback<void, input_driver::SharedEvent&, size_t> callback);

        int start();

        input_driver::SharedEvent& event(size_t position);

    private:

        static constexpr char DELEGATE_FOLDER[] = "/evdev_delegate_devices";
        static constexpr u_int SHMEM_BUFF_LENGTH = 1024;

        typedef struct {
            sem_t server;
            sem_t client;
            size_t current_pos;
            SharedEvent buffy[SHMEM_BUFF_LENGTH];
        } SharedMemoryStruct;

        SharedMemoryStruct *shmp = nullptr;
        /* set .delegated to >=1 to capture the event */
        callback<void, input_driver::SharedEvent&, size_t> _callback;
        int fd = 0;
        std::thread* worker = nullptr;

        [[noreturn]] void loop();

    };

}
