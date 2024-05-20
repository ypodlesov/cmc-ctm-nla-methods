#pragma once
#include "common_container.h"
#include "helpers.h"
#include <random>
#include <iostream>

template <typename T, bool Hold = true>
struct Vector: public CommonContainer<Vector<T>, T, Hold> {
    using Base = CommonContainer<Vector<T>, T, Hold>;

    Vector() = default;

    Vector(int64_t size)
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
};

template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
bool operator ==(const Vector<T>& a, const Vector<T>& b) {
    assert(a.mem_size_ == b.mem_size_);
    T diff = 0;
    for (int64_t i = 0; i < a.mem_size_; ++i) {
        diff += std::fabs(a.data_[i] - b.data_[i]);
    }
    return NHelpers::RoughEq<double>(diff, 0.0, 0.0001);
}

namespace NHelpers {

    template <typename T>
    void GenRandomVector(Vector<T>& a, const int64_t mem_size, bool is_int = false) {
        a = Vector<T>(mem_size);
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_real_distribution<T> real_distribution(0, a.mem_size_ * a.mem_size_);
        for (int64_t i = 0; i < a.mem_size_; ++i) {
            a.data_[i] = is_int ? int(real_distribution(random_engine)) : real_distribution(random_engine);
        }
    }

}

template<typename T>
std::ostream& operator << (std::ostream& os, const Vector<T>& a) {
    os << __PRETTY_FUNCTION__ << std::endl;
    os << "mem_size: " << a.mem_size_ << std::endl;
    os << "vector.data_: ";
    for (int64_t i = 0; i < a.mem_size_; ++i) {
        os << a.data_[i] << ' ';
    }
    os << std::endl;
    return os;
}