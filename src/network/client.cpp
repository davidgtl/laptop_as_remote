#include "client.h"

namespace lap_rem::network {


    void client::loop() {
        using namespace boost::asio;
        using namespace std;


        boost::asio::io_service ios;

        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("192.168.1.105"), port);

        boost::asio::ip::tcp::socket socket(ios);

        socket.connect(endpoint);


        boost::array<char, 128> buf{};

        string message;
        boost::system::error_code error;

        vector<char> buffy;
        while (true) {
           // cin >> message;

            socket.send(boost::asio::buffer(message));
            //cout << "sent: " << message << "\n";

            if (message == "exit")
                break;
        }
        //std::copy(message.begin(),message.end(),buf.begin());
        //socket.write_some(boost::asio::buffer(message), error);
        socket.close();
    }

    void client::broadcasst_search_server() {


    }
}
