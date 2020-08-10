#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <cstring>
#include <fstream>
#include "util/devicewatcher.h"
#include "input/linux_structs.h"


namespace laprem::input {

    typedef struct device_info {
        char name[80];
        bus_types bus_type;
        uint16_t vendor;
        uint16_t product;
        uint16_t version;
        bool events[event_types::EV_CNT];
        bool key_bits[event_code::key::KEY_CNT];
        bool rel_bits[event_code::rel::REL_CNT];
        bool abs_bits[event_code::abs::ABS_CNT];
        bool msc_bits[event_code::msc::MSC_CNT];
        bool led_bits[event_code::led::LED_CNT];
        bool snd_bits[event_code::snd::SND_CNT];
        //bool ff_bits[event_code::keys::KEY_CNT];
        bool sw_bits[event_code::sw::SW_CNT];
        //bool prop_bits[event_code::keys::KEY_CNT];

    } device_info;

    typedef struct device_descriptor {
        std::string unique;
        std::string logical_location;
        std::string physical_location;
        std::string nickname;
        u_int32_t uid;
        device_info device_info;


        bool operator==(const device_descriptor &rhs) const {
            const auto *ptr = dynamic_cast<const device_descriptor *>(&rhs);
            if (ptr != nullptr)
                return this->device_info.vendor == rhs.device_info.vendor &&
                       this->device_info.product == rhs.device_info.product &&
                       this->device_info.name == rhs.device_info.name;

            return false;
        }
        bool operator!=(const device_descriptor &rhs) const {
            const auto *ptr = dynamic_cast<const device_descriptor *>(&rhs);
            if (ptr != nullptr)
                return !(this->device_info.vendor == rhs.device_info.vendor &&
                       this->device_info.product == rhs.device_info.product &&
                       this->device_info.name == rhs.device_info.name);

            return true;
        }
    } device_descriptor;


    class devices {
    private:

        inline static uint32_t lookup_event = 0; // linux doc says max 32 events
        inline static std::set<uint32_t> lookup_fallback = {};
        inline static std::unordered_map<u_int, device_descriptor> _devices = {};
        inline static std::vector<device_descriptor> watched = {};

        static void device_changed(int id, devicewatcher::status status);

        static void update_device(int id);
        inline static constexpr char path[] = "/del_dev/input/";
        inline static constexpr char config_folder[] = "~/.laptop_remote/";
        inline static constexpr char config_watches[] = "wacthes.conf";
        inline static devicewatcher dw{path, device_changed}
        ; // TODO: stop me

        static void build_ids();

        static void populate(device_descriptor &dd, std::string &line) {
            int split = line.find(':');
            auto property = line.substr(0, split);
            auto value = line.substr(split + 1);

            if (property == "Name")
                strncpy(dd.device_info.name, value.c_str(), sizeof(dd.device_info.name));
            else if (property == "Unique")
                dd.unique = value;
            else if (property == "Vendor")
                dd.device_info.vendor = std::stoi(value);
            else if (property == "Product")
                dd.device_info.product = std::stoi(value);
            else if (property == "Logical location")
                dd.logical_location = value;
            else if (property == "Physical location")
                dd.physical_location = value;
            else if (property == "Bus type")
                dd.device_info.bus_type = static_cast<bus_types>(std::stoi(value));
            else if (property == "Firmware version")
                dd.device_info.version = std::stoi(value);
            else if (property == "* Device unique nickname")
                dd.nickname = value;
            else std::cout << "Could not find property " << property << "\n";
            //TODO: read device bits

        }

    public:
        static void query();
        static void watch(int id);
        static bool iswatched(int id);
        static void savewatched();
        static void loadwatched();
    };
}

