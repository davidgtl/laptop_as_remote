#include "event_logic.h"

namespace laprem::logic {

    event_logic::event_logic(std::unordered_map<u_int, input::device_descriptor>::iterator it) :
            event_queue() {
        event_queue.reserve(12);
    }


}
