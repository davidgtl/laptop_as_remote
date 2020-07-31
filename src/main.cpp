#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/program_options.hpp>
#include "input/devices_descriptors.h"
#include "util/devicewatcher.h"

namespace po = boost::program_options;

const int port = 2222;

void server() {
    using namespace boost::asio;
    using namespace boost::asio::ip;
    using namespace std;

    cout << "starting server..\n";


    boost::asio::io_service ios;
    tcp::acceptor acceptor(ios, tcp::endpoint(ip::address_v6::any(), port));
    tcp::socket socket(ios);

    acceptor.accept(socket);

    cout << "client connected\n";

    strerror(errno);


    vector<char> buffy(2048);

    while (true) {
        int msg_len = socket.receive(boost::asio::buffer(buffy));
        string message(buffy.begin(), buffy.begin() + msg_len);
        cout << "received: " << message << "\n";

        if (message == "exit")
            break;
    }

    /*using namespace boost::asio;
    ip::tcp::acceptor acceptor(my_io_context, my_endpoint);
    ip::tcp::socket socket(my_io_context);
    acceptor.accept(socket);*/
    socket.close();
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

void device_changed(int id, devicewatcher::status status) {

    switch (status) {
        case devicewatcher::CREATED:
            printf("device %d was created\n", id); break;
        case devicewatcher::MODIFIED:
            printf("device %d was modified\n", id); break;
        case devicewatcher::DELETED:
            printf("device %d was deleted\n", id); break;
    }
}

void test() {

    lap_rem::input::devices::query();

    devicewatcher fw(device_changed);
    fw.start();

    sleep(20);

    fw.stop();

}

int main(int ac, char *av[]) {
    using namespace std;

    int opt;
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("server,s", "start listening for clients")
            ("client,c", "start search and connect to a local server");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("server"))
        server();
    else if (vm.count("client"))
        client();
    else test();

    return 0;
}
