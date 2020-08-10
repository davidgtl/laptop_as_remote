#include "server.h"

namespace laprem::network {

    template<typename T>
    void server<T>::loop() {
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
                    // TODO: spam test to see if more than one send can be received
                    int msg_len = socket.receive(boost::asio::buffer(buffy));
                    _callback({buffy.data(), msg_len});

                } catch (std::exception const &ex) {
                    break;
                }
            }

            //socket.close();
        }
    }

    template<typename T>
    void server<T>::start() {
        worker = new std::thread(&server::loop, this);
    }

    template<typename T>
    server<T>::server(int port, callback<void, on_message_args&> callback)
            : port(port), _callback(callback) {}

}