#pragma once
#include "common.h"

template <typename T, bool Hold = true>
struct Vector: public CommonContainer<Vector<T>, T, Hold> {
    using Base = CommonContainer<Vector<T>, T, Hold>;
    using Base::data_;

    Vector(size_t size = 0)
        : Base(size)
        , size_{size}
    {
    }

    ~Vector() {
        if (Hold) {
            delete[] data_;
        }
    }

    size_t size_;
};