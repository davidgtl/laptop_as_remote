#include "server.h"

namespace lap_rem::network {

    void server::loop(){
        using namespace boost::asio;
        using namespace boost::asio::ip;
        using namespace std;

        vector<char> buffy(2048);

        boost::asio::io_service ios;
        tcp::acceptor acceptor(ios, tcp::endpoint(ip::address_v6::any(), port));
        tcp::socket socket(ios);

        while (true) {

            acceptor.accept(socket);


            while (true) {
                try {
                    int msg_len = socket.receive(boost::asio::buffer(buffy));
                    string message(buffy.begin(), buffy.begin() + msg_len);

                    std::cout << "received: " << message << "\n";

                    if (message == "exit")
                        break;
                } catch (std::exception const &ex) {
                    break;

                }
            }

            socket.close();
        }
    }

    void server::start() {

    }

    server::server(int port) {
        this->port = port;
    }

}