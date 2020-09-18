#pragma once
#include "linux_structs.h"
#include "input_driver.h"
#include <json.hpp>

namespace laprem::input{
   /* void to_json(nlohmann::json& j, const struct timeval& t) {
        j = nlohmann::json{{"s", t.tv_sec}, {"ms", t.tv_usec}};
    }

    void from_json(const nlohmann::json& j, struct timeval& t) {
        j.at("s").get_to(p.name);
        j.at("ms").get_to(p.address);
        j.at("age").get_to(p.age);
    }
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(struct timeval, tv_sec, tv_usec)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(input_event, time, type, code, value)*/

   void to_json(nlohmann::json& j, const struct input_event& e) {
       j = nlohmann::json{{"time_s", e.time.tv_sec}, {"time_us", e.time.tv_usec}, {"type", e.type}, {"code", e.code}, {"value", e.value}};
   }

    void from_json(const nlohmann::json& j, struct input_event& e) {
        j.at("time_s").get_to(e.time.tv_sec);
        j.at("time_us").get_to(e.time.tv_usec);
        j.at("type").get_to(e.type);
        j.at("code").get_to(e.code);
        j.at("value").get_to(e.value);
    }
}
