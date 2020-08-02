#pragma once

#ifndef LIB_CALLBACK_NAMESPACE
#define LIB_CALLBACK_NAMESPACE gcallbk
#endif

namespace LIB_CALLBACK_NAMESPACE {

    template<typename Return, typename ...Args>
    class callback {
    private:
        typedef Return (*Generic_Method)(void *ref, Args...);
        void *ref;
        void *func;

    public:
        template<typename Class>
        using Method = Return (Class::*)(Args...);
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

#undef LIB_CALLBACK_NAMESPACE


