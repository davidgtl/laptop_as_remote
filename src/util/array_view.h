#pragma once

#include <vector>

namespace lap_rem {

    template<typename T>
    class array_view {
        T *ptr;
        std::size_t length;
    public:
        array_view(T *ptr, std::size_t len) noexcept: ptr{ptr}, length{len} {}

        T &operator[](int i) noexcept { return ptr[i]; }

        T const &operator[](int i) const noexcept { return ptr[i]; }

        auto size() const noexcept { return length; }

        auto begin() noexcept { return ptr; }

        auto end() noexcept { return ptr + length; }
    };
}

