#include "callbacks.h"

namespace tests::callbacks {

    void global_addtwo(int k){
        std::cout << k << " + 2 = " << k + 2 << "\n";

    }

    class A {
    public:
        explicit A(const callback<void, int> &cb) {
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

            method_callback cb(*this, &B::addtwo);
            cb(7);

            method_callback cb_wrong(*this, &B::addu);
            cb_wrong(8);

            A a1(cb); // works - pass
            //A(cb_wrong); //error - pass
            A a2(method_callback(*this, &B::addtwo)); // works - pass
            //A(method_callback(*this, &B::addu)); // error - pass
            A a3((function_callback(global_addtwo)));


        }
    };

    void test1() {

        B b;

    }
}
