#include "client.h"
#include "input/devices_descriptors.h"
#include <thread>
#include "message.hpp"

namespace laprem::logic {
    using namespace input;

    client::client() :
            event_queue(1024), input_driver({*this, &client::on_input}), emulator() {

    }

    void client::start() {
        devices::query();
        devices::loadwatched();

        input_driver.start(); //TODO: conf and routing =

        using namespace network;
        discovery d(57897);
        boost::asio::ip::address addr;
        if (d.search_listeners("server1", addr)) {
            std::cout << "Server found\n";

            network::client client(addr, 57896);
            client.connect();
            //client.send("hello");
            std::unique_lock <std::mutex> lock(input_mutex);

            while (true) {
                lock.lock();
                if (event_queue.empty())
                    continue;

                input::input_driver::SharedEvent &sev =
                        input_driver.event(event_queue.pop());

                send_message(client, sev.ev);
            }
        } else
            std::cout << "Server not found\n";


    }

    bool filter_events(input::input_driver::SharedEvent &sev) {
        static bool capture = false;
        if (sev.ev.type == event_types::EV_KEY &&
            sev.ev.code == event_code::key::KEY_PAUSE) { // TODO: config it
            capture = !capture;
            printf("capture %s\n", capture ? "on" : "off");
            return true;
        }
        return capture;
    }


    void client::on_input(input::input_driver::SharedEvent &sev, size_t position) {
        if (devices::iswatched(sev.device_identifier) && filter_events(sev)) {
            sev.delegated = 1;
            event_queue.push(position);
            std::unique_lock <std::mutex> lock(input_mutex);
            lock.unlock();
        }
    }

    void client::stop() {
        input::devices::savewatched();
    }
};
