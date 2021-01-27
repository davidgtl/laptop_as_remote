#pragma once

#include "callback.hpp"

namespace laprem {

    template<typename ReturnPack, typename ArgPack, typename Enum = u_int32_t>
    class logic_junction {
        std::vector<callback<ReturnPack, ArgPack>> functions;
        Enum choice = 0;

    public:
        void set_choice(Enum value){
            choice = value;
        }

        ReturnPack operator()(ArgPack args){
            return functions[(u_int32_t) choice](args);
        }

    };

}


