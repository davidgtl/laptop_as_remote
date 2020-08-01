#include <iostream>
#include <boost/program_options.hpp>
#include "input/devices_descriptors.h"
#include "util/devicewatcher.h"

namespace po = boost::program_options;
namespace lap_rem {
    const int port = 2222;

    void server() {

    }

    void client() {
        using namespace boost::asio;
        using namespace std;

        cout << "starting client..\n";

        boost::asio::io_service ios;

        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("192.168.1.105"), port);

        boost::asio::ip::tcp::socket socket(ios);

        socket.connect(endpoint);

        cout << "server connected\n";

        boost::array<char, 128> buf{};

        string message;
        boost::system::error_code error;

        vector<char> buffy;
        while (true) {
            cin >> message;

            socket.send(boost::asio::buffer(message));
            cout << "sent: " << message << "\n";

            if (message == "exit")
                break;
        }
        //std::copy(message.begin(),message.end(),buf.begin());
        //socket.write_some(boost::asio::buffer(message), error);
        socket.close();
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

        devicewatcher fw(device_changed);
        fw.start();

        sleep(20);

        fw.stop();

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
