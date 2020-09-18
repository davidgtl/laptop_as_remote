#pragma once

//#include <boost/iostreams/stream.hpp>
//#include <boost/iostreams/stream_buffer.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>

#include <json.hpp>
#include <input/linux_structs.h>
#include <input/devices_descriptors.h>
#include <network/client.h>

/*
BOOST_IS_BITWISE_SERIALIZABLE(laprem::input::input_event)
BOOST_IS_BITWISE_SERIALIZABLE(laprem::input::input_driver::SharedEvent)
BOOST_IS_BITWISE_SERIALIZABLE(laprem::input::device_info)
BOOST_IS_BITWISE_SERIALIZABLE(laprem::input::device_descriptor)
*/

namespace laprem::logic {

    typedef enum : uint16_t{
        DEVICE_DESCRIPTOR = 0x1,
        INPUT_EVENT = 0x2
    } message_type;

    template<typename T>
    struct message;

    template<>
    struct message<laprem::input::device_descriptor>{
        inline static message_type type = DEVICE_DESCRIPTOR;
    };

    template<>
    struct message<laprem::input::input_driver::SharedEvent>{
        inline static message_type type = INPUT_EVENT;
    };

    template<typename T>
    void send_message(const network::client& c, const T& data){
        using namespace nlohmann;

        json jdata = data;
        json jmessage;
        jmessage["type"] = message<T>::header;
        jmessage["data"] = jdata;
        std::vector<std::uint8_t> buffy = json::to_msgpack(jmessage);

        c.send_buffer(buffy);
    }

}


