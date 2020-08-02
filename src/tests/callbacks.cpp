#include "callbacks.h"
#include <string>
#include <type_traits>

namespace tests::callbacks {

    void global_addtwo(int k) {
        std::cout << k << " + 2 = " << k + 2 << "\n";

    }

    int global_raddtwo(int k) {
        std::cout << k << " + 2 = " << k + 2 << "\n";
        return k;
    }

    class A {
    public:
        A(callback<void, int> cb) {
            cb(100);
        }
    };

    class B {
    public:
        void addtwo(int k) {
            std::cout << k << "+2=" << k + 2 << "\n";
        }

        void addu(char u) {
            std::cout << u << "\n";
        }

        explicit B() {
            typedef void (B::*Member)(int);
            typedef void (*Raw_Member)(B *, int);
            Member m = &B::addtwo;
            (this->*m)(5);

            callback cb(*this, &B::addtwo);
            cb(7);

            callback cb_wrong(*this, &B::addu);
            cb_wrong('k');

            callback global(global_addtwo);
            global(101);

            A a1(cb); // works - pass
            //A(cb_wrong); //error - pass
            A a2({*this, &B::addtwo}); // works - pass
            //A({*this, &B::addu}); // error - pass
            A a3(global_addtwo);  // works - pass
            //A a4(global_raddtwo); // error - pass


        }
    };

    void test1() {
        B b;
    }

    template<typename F, typename C>
    class Base {
        F f;
        C c;
    public:
        Base(F f, C c) : f(f), c(c) {
            std::cout << "FC\n";
        }
    };

    template<typename F>
    class Base<F, void> {
        F f;
    public:
        Base(F f) : f(f) {
            std::cout << "F\n";
        }

    };

    template<typename F> Base(F) -> Base<F, void>;
    template<typename F, typename C> Base(F, C) -> Base<F, C>;

    void test2() {
        Base b1(1);
        Base b2("hello", 1);
    }
}
