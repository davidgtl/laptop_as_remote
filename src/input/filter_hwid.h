#pragma once

#include "input_driver.h"
#include "util/devicewatcher.h"

namespace lap_rem::input {

    class filter_hwid : Icapture_logic {

    private:
        typedef struct{
            std::string name;
            u_int32_t vendor;
            u_int32_t product;
            std::string unique_nickname;
        } logical_device;

    public:
        filter_hwid();

    };

}