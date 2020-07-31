#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <semaphore.h>

#include "input_driver.h"

namespace lap_rem::input {
    int input_driver::init() {
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

        return 0;

    }

    void input_driver::next_event(input_driver::SharedEvent &ev) {
        sem_wait(&shmp->server); // wait for event from driver


    }

    void input_driver::post_result(bool handled) {

        sem_post(&shmp->client); // notify driver that logic processing is done
    }

}