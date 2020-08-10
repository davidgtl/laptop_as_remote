#pragma once

#include <vector>

namespace laprem {

    template<typename T>
    class queue {
    private:
        std::vector<T> buffy;
        size_t head, tail;
    public:
        queue(size_t size = 16) : buffy(size), head(0), tail(0) {
        }

        void push(T &obj) {
            buffy[head] = obj;
            head++;
            if (head >= buffy.size() && tail > 0)
                head = 0;
            else if (head >= buffy.size() && tail == 0)
                buffy.resize(buffy.size() * 2);
            else if (head == tail){
                size_t diff = buffy.size();
                buffy.resize(buffy.size() * 2);
                for(int i = buffy.size() - 1; i >= tail + diff; i--)
                    buffy[i] = buffy[i - diff];
                tail += diff;
            }
        }

        T& pop(){
            T& elem = buffy[tail];
            tail++;
            if(tail >= buffy.size())
                tail = 0;
        }

        bool empty() {
            return head == tail;
        }
    };
}



