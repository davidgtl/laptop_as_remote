#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <semaphore.h>

#include "input_driver.h"

namespace laprem::input {

    input_driver::input_driver(laprem::callback<void, input_driver::SharedEvent &, size_t> callback)
            : _callback(callback) {}

    int input_driver::start() {
        fd = shm_open(DELEGATE_FOLDER, O_RDWR, 0);
        if (fd == -1) {
            printf("fd error\n");
            return 1;
        }

        shmp = (SharedMemoryStruct *) mmap(nullptr, sizeof(SharedMemoryStruct),
                                           PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (shmp == MAP_FAILED) {
            printf("mmap error");
            return 2;
        }

        sem_wait(&shmp->server); // let driver know we are ready for events

        worker = new std::thread(&input_driver::loop, this);
        return 0;

    }

    [[noreturn]] void input_driver::loop() {
        while (true) {
            sem_wait(&shmp->server); // wait for event from driver
            shmp->buffy[shmp->current_pos].delegated = 0;
            _callback(shmp->buffy[shmp->current_pos], shmp->current_pos);
            sem_post(&shmp->client);
        }
    }

    input_driver::SharedEvent &input_driver::event(size_t position) {
        return shmp->buffy[position];
    }

}