#include <iostream>
#include <boost/program_options.hpp>
#include "util/devicewatcher.h"
#include "network/network.h"
#include "tests/callbacks.h"
#include "input/emulator.h"

namespace po = boost::program_options;
namespace laprem {
    const int port = 2222;

    void server() {
        using namespace network;
        discovery d(57897);
        d.start_listener("server1");
        std::string line;
        std::cin >> line;
    }

    void client() {


    }

    void config() {
        /*using namespace input;
        devices::query();

        std::cout << "Please enter the ids of devices to monitor:\n";

        std::string input;
        getline(std::cin, input); // get input until newline

        std::istringstream sstr(input);
        int id;
        while (sstr >> id)
            devices::watch(id);*/


    }

    void test() {
        using namespace std::literals::chrono_literals;
        //lap_rem::input::devices::query();

        /*devicewatcher fw(device_changed);
        fw.start();

        sleep(20);

        fw.stop();*/
        printf("starting...\n");
        input::emulator emu;
        emu.start();
        std::cin.get();
        //std::this_thread::sleep_for(3000ms);
        emu.send_key(0, true);
        std::this_thread::sleep_for(50ms);
        emu.send_key(0, false);
        std::this_thread::sleep_for(1000ms);
        emu.send_key(0, true);
        std::this_thread::sleep_for(50ms);
        emu.send_key(0, false);
        emu.stop();
        printf("stopping...\n");

    }
}

int main(int ac, char *av[]) {
    using namespace std;
    using namespace laprem;

    int opt;
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("server,s", "start listening for clients")
            ("client,c", "start search and connect to a local server");
    ("config", "choose which devices to monitor");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("server"))
        server();
    else if (vm.count("client"))
        client();
    else if (vm.count("configure"))
        config();
    else test();

    return 0;
}
