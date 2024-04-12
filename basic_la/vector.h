#pragma once
#include "common_container.h"

template <typename T, bool Hold = true>
struct Vector: public CommonContainer<Vector<T>, T, Hold> {
    using Base = CommonContainer<Vector<T>, T, Hold>;
    using Base::data_;
    using Base::mem_size_;

    Vector(size_t size = 0)
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

    template <typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    inline T Norm2() const noexcept {
        T result{};
        for (size_t i = 0; i < mem_size_; ++i) {
            result += data_[i] * data_[i];
        }
        return result;
    }

    ~Vector() {
        if (Hold) {
            delete[] data_;
        }
    }
};

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
bool operator ==(const Vector<T>& a, const Vector<T>& b) {
    if (a.mem_size_ != b.mem_size_) {
        return false;
    }
    T diff{};
    for (size_t i = 0; i < a.mem_size_; ++i) {
        diff += std::fabs(a.data_[i] - b.data_[i]);
    }
    return NHelpers::RoughEq<double>(diff, 0.0, 0.0001);
}