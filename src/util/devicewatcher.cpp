#include "devicewatcher.h"
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <util/callback.hpp>

#define watcher ((pthread_t *) _watcher)

namespace lap_rem::input {

    static constexpr u_int EVENT_SIZE = sizeof(struct inotify_event);
    static constexpr u_int BUF_LEN = 1024 * (EVENT_SIZE + 64);


    void *loop(void* arg) {
        auto* ref = (devicewatcher*)arg;

        char buffer[BUF_LEN];

        while (ref->fd >= 0) { // useless condition to get a big yellow warning out of my face
            int length = read(ref->fd, buffer, BUF_LEN);

            if (length < 0) printf("inotify read error");

            for (int i = 0; i < length;) {
                struct inotify_event *event = (struct inotify_event *) &buffer[i];

                if (!event->len || event->mask & IN_ISDIR) continue;

                if (event->mask & IN_CREATE)
                    ref->_callback(atoi(event->name), devicewatcher::status::CREATED);
                else if (event->mask & IN_DELETE)
                    ref->_callback(atoi(event->name), devicewatcher::status::DELETED);
                else if (event->mask & IN_MODIFY)
                    ref->_callback(atoi(event->name), devicewatcher::status::MODIFIED);

                i += EVENT_SIZE + event->len;
            }
        }
    }

    void devicewatcher::start() {
        if (watcher != nullptr)
            return;

        fd = inotify_init();
        if (fd < 0) {
            printf("inotify_init error\n");

        }

        wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);

        _watcher = new pthread_t;
        int rc = pthread_create(watcher, nullptr, &loop, (void *) this);
        if (rc)
            printf("pthread_create error code: %d\n", rc);

    }

    void devicewatcher::stop() {
        if (watcher == nullptr)
            return;

        int rc;

        rc = pthread_cancel(*watcher);

        if (rc != 0)
            printf("pthread_cancel error code %d", rc);

        void *res;
        rc = pthread_join(*watcher, &res);
        if (rc != 0)
            printf("pthread_join error code %d", rc);

        if (res != PTHREAD_CANCELED)
            printf("thread was not canceled normally");


        inotify_rm_watch(fd, wd);
        close(fd);
    }

    devicewatcher::devicewatcher(const std::string &path, lap_rem::callback<void, int, status> callback)
            : _callback(callback), path(path) {}

}
