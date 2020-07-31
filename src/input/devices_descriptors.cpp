#include "devices_descriptors.h"

namespace lap_rem::input {

    void devices::device_changed(int id, devicewatcher::status status) {

        switch (status) {
            case devicewatcher::CREATED:
                printf("device %d was created\n", id);
                break;
            case devicewatcher::MODIFIED:
                printf("device %d was modified\n", id);
                break;
            case devicewatcher::DELETED:
                printf("device %d was deleted\n", id);
                break;
        }
    }


    void devices::query() {
        devices::_devices.clear();

        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            int id = std::stoi(entry.path().filename());
            printf("Found device %d:\n", id);

            device_descriptor dd;

            std::ifstream file(entry.path());
            std::string line;
            while (std::getline(file, line))
                populate(dd, line);

            printf("\n");

            devices::_devices[id] = dd;
        }

        dw.start();
    }
}