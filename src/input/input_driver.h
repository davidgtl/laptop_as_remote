#pragma once

#include <semaphore.h>
#include <string>
#include <vector>

namespace lap_rem::input {

    class Icapture_logic {
    public:
        virtual bool capture() = 0;
    };


    class input_driver {

    public:

        struct input_event {
#if (__BITS_PER_LONG != 32 || !defined(__USE_TIME_BITS64)) && !defined(__KERNEL__)
            struct timeval time;
#define input_event_sec time.tv_sec
#define input_event_usec time.tv_usec
#else
            __kernel_ulong_t __sec;
#if defined(__sparc__) && defined(__arch64__)
        unsigned int __usec;
        unsigned int __pad;
#else
        __kernel_ulong_t __usec;
#endif
#define input_event_sec  __sec
#define input_event_usec __usec
#endif
            unsigned short type;
            unsigned short code;
            int value;
        };

        typedef struct {
            int device_identifier;
            u_char delegated;
            struct input_event ev;
        } SharedEvent;

        input_driver() = default;

        int init();

        void next_event(SharedEvent &ev);

        void post_result(bool handled);

    private:

        static constexpr char DELEGATE_FOLDER[] = "/evdev_delegate_devices";
        static constexpr u_int SHMEM_BUFF_LENGTH = 1024;

        typedef struct {
            sem_t server;
            sem_t client;
            size_t current_pos;
            SharedEvent buffy[SHMEM_BUFF_LENGTH];
        } SharedMemoryStruct;

        SharedMemoryStruct *shmp;
        int fd;

    };
}
