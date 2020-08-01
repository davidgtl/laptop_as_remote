#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <cstring>
#include <fstream>
#include "util/devicewatcher.h"


namespace lap_rem::input {

    typedef struct _device_descriptor {
        std::string name;
        std::string unique;
        u_int32_t vendor;
        u_int32_t product;
        std::string logical_location;
        std::string physical_location;
        u_int32_t bus_type;
        u_int32_t firmware_version;

        std::string nickname;
        u_int32_t uid;



        bool operator==(const _device_descriptor &rhs) const {
            const auto *ptr = dynamic_cast<const _device_descriptor *>(&rhs);
            if (ptr != nullptr)
                return this->vendor == rhs.vendor &&
                       this->product == rhs.product &&
                       this->name == rhs.name;

            return false;
        }
        bool operator!=(const _device_descriptor &rhs) const {
            const auto *ptr = dynamic_cast<const _device_descriptor *>(&rhs);
            if (ptr != nullptr)
                return !(this->vendor == rhs.vendor &&
                       this->product == rhs.product &&
                       this->name == rhs.name);

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
        inline static devicewatcher dw{path, device_changed};

        static void build_ids();

        static void populate(device_descriptor &dd, std::string &line) {
            int split = line.find(':');
            auto property = line.substr(0, split);
            auto value = line.substr(split + 1);

            if (property == "Name")
                dd.name = value;
            else if (property == "Unique")
                dd.unique = value;
            else if (property == "Vendor")
                dd.vendor = std::stoi(value);
            else if (property == "Product")
                dd.product = std::stoi(value);
            else if (property == "Logical location")
                dd.logical_location = value;
            else if (property == "Physical location")
                dd.physical_location = value;
            else if (property == "Bus type")
                dd.bus_type = std::stoi(value);
            else if (property == "Firmware version")
                dd.firmware_version = std::stoi(value);
            else if (property == "* Device unique nickname")
                dd.nickname = value;
            else std::cout << "Could not find property " << property << "\n";

        }

    public:
        static void query();
        static void watch(int id);
        static bool iswatched(int id);
        static void savewatched();
        static void loadwatched();
    };
}

