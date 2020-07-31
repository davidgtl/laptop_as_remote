#include "devicewatcher.h"
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>

static constexpr u_int EVENT_SIZE = sizeof(struct inotify_event);
static constexpr u_int BUF_LEN = 1024 * (EVENT_SIZE + 64);

#define watcher ((pthread_t *) _watcher)

[[noreturn]] void *worker(void *arg);

void devicewatcher::start() {
    if (watcher != nullptr)
        return;

    _internal.fd = inotify_init();
    if (_internal.fd < 0) {
        printf("inotify_init error\n");

    }

    _internal.wd = inotify_add_watch(_internal.fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);

    _watcher = new pthread_t;
    int rc = pthread_create(watcher, nullptr, worker, (void *) this);
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


    inotify_rm_watch(_internal.fd, _internal.wd);
    close(_internal.fd);
}

devicewatcher::devicewatcher(const std::string &path, void (*callback)(int, status)) {
    this->_internal.callback = callback;
    this->path = path;
}

[[noreturn]] void *worker(void *arg) {

    auto *fw = (devicewatcher *) arg;
    int fd = fw->_internal.fd;
    char buffer[BUF_LEN];

    while (true) {
        int length = read(fd, buffer, BUF_LEN);

        if (length < 0) printf("inotify read error");

        for (int i = 0; i < length;) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];

            if (!event->len || event->mask & IN_ISDIR) continue;

            if (event->mask & IN_CREATE)
                fw->_internal.callback(atoi(event->name), devicewatcher::status::CREATED);
            else if (event->mask & IN_DELETE)
                fw->_internal.callback(atoi(event->name), devicewatcher::status::DELETED);
            else if (event->mask & IN_MODIFY)
                fw->_internal.callback(atoi(event->name), devicewatcher::status::MODIFIED);

            i += EVENT_SIZE + event->len;
        }
    }

}
