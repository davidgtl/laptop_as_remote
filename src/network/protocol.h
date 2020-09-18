#include <tink/config/tink_config.h>
#include <tink/public_key_sign.h>
#include <tink/public_key_sign_factory.h>
#include <boost/asio.hpp>

namespace laprem::network {

    class Iprotocol{
    public:
        virtual void message_callback(const std::vector<char>& buffer,  const boost::asio::ip::udp::endpoint& sender) = 0;
    };

    class protocol : public Iprotocol{
    public:
        void message_callback(const std::vector<char>& buffer,  const boost::asio::ip::udp::endpoint& sender) override;


    };

}