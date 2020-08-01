#include "discovery.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace lap_rem::network {

    discovery::discovery(int port) {
        this->port = port;
    }

    void discovery::start_listener(const std::string& reply) {
        this->reply = reply;
        worker = new std::thread(&discovery::loop_listener, this);
    }

    void discovery::loop_listener() {

        using namespace boost::asio;
        using namespace boost::asio::ip;
        using namespace std;

        vector<char> buffy(2048);

        udp::endpoint remote_endpoint;

        boost::system::error_code error;
        boost::asio::io_service ios;
        udp::socket socket(ios, udp::endpoint(udp::v4(), port));


        while (true) {
            try {
                int msg_len = socket.receive_from(boost::asio::buffer(buffy), remote_endpoint, 0, error);

                cout << "Client connected: " << boost::lexical_cast<std::string>(remote_endpoint) << "\n";
                cout << "Received: " << std::string(buffy.begin(), buffy.begin() + msg_len) << "\n";

                if (std::string(buffy.begin(), buffy.begin() + msg_len) != "searching") {
                    continue;
                }

                socket.send_to(boost::asio::buffer(reply), remote_endpoint);

            } catch (std::exception const &ex) {
                break;
            }
        }

        //socket.close();


    }

    void discovery::search_listeners() {

        using namespace boost::asio;
        using namespace boost::asio::ip;
        using namespace std;
        using namespace std::chrono_literals;

        io_service ios;
        boost::system::error_code error;

        udp::endpoint broadcast_endpoint(address_v4::broadcast(), port), remote_endpoint;
        udp::socket socket(ios);
        socket.open(udp::v4(), error);
        socket.set_option(udp::socket::reuse_address(true));
        socket.set_option(socket_base::broadcast(true));

        string message = "searching";

        vector<char> buffy(2048);
        int current_timeout = 1; //1ms
        for (int i = 0; i <= 6; i++) { //up to 64ms round trip

            socket.send_to(boost::asio::buffer(message), broadcast_endpoint);
            std::this_thread::sleep_for(current_timeout * 1ms);

            if (socket.available()) {
                cout << "ping: " << current_timeout << "ms\n";
                int msg_len = socket.receive_from(boost::asio::buffer(buffy), remote_endpoint, 0, error);
                cout << "Server connected: " << boost::lexical_cast<std::string>(remote_endpoint) << "\n";
                cout << "Received from server: " << std::string(buffy.begin(), buffy.begin() + msg_len) << "\n";
                break;
            }
            current_timeout *= 2;
        }

        socket.close();

    }
}
