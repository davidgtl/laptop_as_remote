#include "devices_descriptors.h"
#include <algorithm>
#include <filesystem>

namespace laprem::input {

    void devices::device_changed(int id, devicewatcher::status status) {
        devices::query();
        devices::build_ids();

        switch (status) {
            case devicewatcher::CREATED:
            case devicewatcher::MODIFIED:
                update_device(id);
                break;
            case devicewatcher::DELETED:
                _devices.erase(id);
                break;
        }
    }


    void devices::update_device(int id) {
        std::ifstream file(path + std::to_string(id));

        device_descriptor dd;
        std::string line;
        while (std::getline(file, line))
            populate(dd, line);

        dd.uid = id;
        devices::_devices[id] = dd;
        build_ids();
    }


    void devices::query() {
        devices::_devices.clear();

        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            int id = std::stoi(entry.path().filename());

            device_descriptor dd;

            std::ifstream file(entry.path());
            std::string line;
            while (std::getline(file, line))
                populate(dd, line);

            dd.uid = id;
            devices::_devices[id] = dd;
        }

        dw.start();
    }

    void devices::watch(int id) {
        if (std::find(watched.begin(), watched.end(), _devices[id]) != watched.end())
            watched.push_back(_devices[id]);
    }

    void devices::savewatched() {
        namespace fs = std::filesystem;

        if (!fs::is_directory(config_folder) || !fs::exists(config_folder))
            fs::create_directory(config_folder);

        std::ofstream conf(config_folder + std::string(config_watches));

        int cid = 0;
        for (auto &e: watched) {
            conf << "* Device unique nickname:device" << cid << "\n";
            conf << "Name:" << e.device_info.name << "\n";
            conf << "Vendor:" << e.device_info.vendor << "\n";
            conf << "Product:" << e.device_info.product << "\n";
            conf << "\n";
        }

    }

    void devices::loadwatched() {
        namespace fs = std::filesystem;

        if (!fs::is_directory(config_folder) || !fs::exists(config_folder)) {
            std::ofstream conf(config_folder + std::string(config_watches));
        }

        std::ifstream conf(config_folder + std::string(config_watches));
        std::string line;
        device_descriptor dd;

        bool populated = false;

        while (std::getline(conf, line)) {

            if (line.length() <= 1) {
                if (populated) {
                    for (auto &it : _devices)
                        if (it.second == dd) {
                            watched.push_back(it.second);
                        }

                    populated = false;
                }
                continue;
            }

            populate(dd, line);
            populated = true;

        }

        build_ids();
    }

    void devices::build_ids() {

        lookup_event = 0;
        lookup_fallback.clear();

        for (auto &w: watched)
            for (auto &it : _devices) {
                if (it.second != w) continue;
                w.uid = it.first;
                if (it.first < 32)
                    lookup_event |= 1 << it.first;
                else
                    lookup_fallback.insert(it.first);
            }

        dw.start();

    }

    bool devices::iswatched(int id) {
        if (id < 32)
            return lookup_event & (1 << id);
        else
            return lookup_fallback.find(id) != lookup_fallback.end();
    }

}

