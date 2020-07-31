#pragma once

#include <string>
#include <vector>
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

    typedef struct {
        std::string name;
        std::string unique;
        u_int32_t vendor;
        u_int32_t product;
        std::string logical_location;
        std::string physical_location;
        u_int32_t bus_type;
        u_int32_t firmware_version;
    } device_descriptor;


    class devices {
    private:

        int watched_ids[31];

        static void device_changed(int id, devicewatcher::status status);

        inline static std::unordered_map<u_int, device_descriptor> _devices = {};
        inline static constexpr char path[] = "/del_dev/input";
        inline static devicewatcher dw{path, device_changed};

        static void populate(device_descriptor &dd, std::string &line) {
            int split = line.find(':');
            auto property = line.substr(0, split);
            auto value = line.substr(split + 1);

            printf("%s: %s\n", property.c_str(), value.c_str());

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
            else std::cout << "Could not find property " << property << "\n";

        }

    public:
        static void query();
    };
}

