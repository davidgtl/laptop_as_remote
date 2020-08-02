#pragma once

#ifndef LIB_CALLBACK_NAMESPACE
#define LIB_CALLBACK_NAMESPACE gcallbk
#endif

namespace LIB_CALLBACK_NAMESPACE {

    template<typename Return, typename ...Args>
    class callback {
    public:
        virtual Return operator()(Args... args) const = 0;
    };


    template<typename Class, typename Return, typename ...Args>
    class method_callback : public callback<Return, Args...> {
    private:
        void *ref;

        typedef Return (*Generic_Method)(void *ref, Args...);

        Return (*method)(void *ref, Args...);

    public:
        typedef Return (Class::*Method)(Args...);

        method_callback(Class &ref, Method m) :
                ref((void *) (&ref)), method((Generic_Method) (ref.*m)) {}

        Return operator()(Args... args) const override {
            return method(ref, args...);
        }

    };

    template<typename Return, typename ...Args>
    class function_callback : public callback<Return, Args...> {
    private:
        Return (*method)(Args...);

    public:
        typedef Return (*Function)(Args...);

        explicit function_callback(Function f) : method(f) {}

        Return operator()(Args... args) const override {
            return method(args...);
        }

    };
}

#undef LIB_CALLBACK_NAMESPACE


