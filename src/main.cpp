#include <iostream>
#include <boost/program_options.hpp>
#include "input/devices_descriptors.h"
#include "util/devicewatcher.h"
#include "network/discovery.h"

namespace po = boost::program_options;
namespace lap_rem {
    const int port = 2222;

    void server() {
        using namespace network;
        discovery d(57897);
        d.start_listener("server1");
        std::string line;
        std::cin>>line;
    }

    void client() {
        using namespace network;
        discovery d(57897);
        d.search_listeners();
        std::string line;
        std::cin>>line;
    }

    void config() {
        using namespace input;
        devices::query();

        std::cout << "Please enter the ids of devices to monitor:\n";

        std::string input;
        getline(std::cin, input); // get input until newline

        std::istringstream sstr(input);
        int id;
        while (sstr >> id)
            devices::watch(id);


    }

    void test() {

        lap_rem::input::devices::query();

        /*devicewatcher fw(device_changed);
        fw.start();

        sleep(20);

        fw.stop();*/

    }
}
int main(int ac, char *av[]) {
    using namespace std;
    using namespace lap_rem;

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
