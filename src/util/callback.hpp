#pragma once

namespace lap_rem {

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
}



