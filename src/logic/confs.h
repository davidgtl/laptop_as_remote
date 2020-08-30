#pragma once

#include <string>
#include <json.hpp>

#define CONST_STRING inline static constexpr std::string_view

namespace laprem::logic::confs {

    CONST_STRING config_folder = "~/.laprem/";

    struct configs{
         CONST_STRING WATCHES = "watches.conf";
         CONST_STRING EMULATED = "emulated.conf";
         CONST_STRING ROUTING = "routing.conf";
    };

    void load(const std::string& file){
        namespace pt = boost::property_tree;
        pt::ptree tree;

        pt::read_json(std::string(config_folder) + file, tree);

        // Use the throwing version of get to find the debug filename.
        // If the path cannot be resolved, an exception is thrown.
        m_file = tree.get<std::string>("debug.filename");

        // Use the default-value version of get to find the debug level.
        // Note that the default value is used to deduce the target type.
        m_level = tree.get("debug.level", 0);

        // Use get_child to find the node containing the modules, and iterate over
        // its children. If the path cannot be resolved, get_child throws.
        // A C++11 for-range loop would also work.
        BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("debug.modules")) {
                        // The data function is used to access the data stored in a node.
                        m_modules.insert(v.second.data());
                    }
    }

    void save(const std::string &filename)
    {
        using json = nlohmann::json;
    }

};

#undef CONST_STRING



