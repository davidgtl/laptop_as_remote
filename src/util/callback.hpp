#pragma once

#include <vector>
#include <algorithm>

namespace laprem {

    template<typename Return, typename ...Args>
    class callback {
    private:
        void *ref;
        void *func;

    public:
        template<typename Class>
        using Method = Return (Class::*)(Args...);

        typedef Return (*Generic_Method)(void *ref, Args...);

        typedef Return (*Function)(Args...);

        template<typename Class>
        callback(Class &ref, Method<Class> m) :
                ref((void *) (&ref)), func((void *) (ref.*m)) {}

        callback(Function f) : ref(nullptr), func((void *) f) {}

        Return operator()(Args... args) const {
            if (ref == nullptr)
                return ((Function) func)(args...);
            else
                return ((Generic_Method) func)(ref, args...);
        }
    };

    template<typename Return, typename ...Args>
    class event {
    private:
        std::vector<callback<Return, Args...> *> callbacks;
    public:
        event() = default;

        void operator+=(const callback<Return, Args...> &callback) {
            callbacks.push_back(&callback);
        }

        void operator-=(const callback<Return, Args...> &callback) {
            callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), &callback),
                    callbacks.end());
        }

        template<std::enable_if_t<std::is_same_v<Return, void>> = 0>
        void operator()(Args... args) const {
            for (auto callback : callbacks)
                if (callback != nullptr)
                    callback();
        }

        /*template<std::enable_if_t<!std::is_same_v<Return, void>> = 0>
        std::vector<Return> operator()(Args... args) const {
            std::vector<Return> results;
            for (auto callback : callbacks)
                if (callback != nullptr)
                    results.push_back(callback());

        }*/

    };
}



