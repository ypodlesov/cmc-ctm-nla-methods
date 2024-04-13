#pragma once
#include "common_container.h"
#include "helpers.h"

template <typename T, bool Hold = true>
struct Vector: public CommonContainer<Vector<T>, T, Hold> {
    using Base = CommonContainer<Vector<T>, T, Hold>;
    using Base::data_;
    using Base::mem_size_;

    Vector() = default;

    Vector(size_t size)
        : Base(size)
    {
    }

    Vector(const Vector& other)
        : Base(other)
    {
    }

    Vector(Vector&& other)
        : Base(std::move(other))
    {
    }

    Vector& operator =(const Vector& other) {
        Base::operator =(other);
        return *this;
    }

    Vector& operator =(Vector&& other) {
        Base::operator =(std::move(other));
        return *this;
    }

    ~Vector() = default;
};

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
bool operator ==(const Vector<T>& a, const Vector<T>& b) {
    assert(a.mem_size_ == b.mem_size_);
    T diff{};
    for (size_t i = 0; i < a.mem_size_; ++i) {
        diff += std::fabs(a.data_[i] - b.data_[i]);
    }
    return NHelpers::RoughEq<double>(diff, 0.0, 0.0001);
}