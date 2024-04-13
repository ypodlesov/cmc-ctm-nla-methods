#pragma once
#include "helpers.h"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <type_traits>
#include <utility>

template <typename ContainerType, typename T, bool Hold = true>
struct CommonContainer {

    CommonContainer() = default;

    CommonContainer(size_t mem_size)
        : mem_size_{mem_size}
        , data_{new T[mem_size_]}
    {
    }

    CommonContainer(const CommonContainer& other) {
        mem_size_ = other.mem_size_;
        data_ = new T[mem_size_];
        for (size_t i = 0; i < mem_size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    CommonContainer(CommonContainer&& other) {
        if (data_ && Hold) {
            delete[] data_;
            data_ = nullptr;
        }
        std::swap(data_, other.data_);
        other.data_ = nullptr;
        std::swap(mem_size_, other.mem_size_);
        other.mem_size_ = 0;
    }

    CommonContainer& operator =(const CommonContainer& other) {
        if (data_ && Hold) {
            delete[] data_;
            data_ = nullptr;
        }
        mem_size_ = other.mem_size_;
        data_ = new T[mem_size_];
        for (size_t i = 0; i < mem_size_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }

    CommonContainer& operator =(CommonContainer&& other) {
        std::swap(data_, other.data_);
        other.data_ = nullptr;
        std::swap(mem_size_, other.mem_size_);
        other.mem_size_ = 0;
        return *this;
    }

    template <typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    void PlusAX(const CommonContainer& x, const T& alpha = 1) {
        assert(mem_size_ == x.mem_size_);
        for (size_t i = 0; i < mem_size_; ++i) {
            data_[i] += alpha * x.data_[i];
        }
    }

    template <typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    void AXPlusBY(const CommonContainer& x, const T& alpha, const CommonContainer& y, const T& beta = 1) {
        assert(x.mem_size_ == y.mem_size_);
        if (data_ && Hold) {
            delete[] data_;
            data_ = nullptr;
        }
        mem_size_ = x.mem_size_;
        data_ = new T[mem_size_];
        for (size_t i = 0; i < mem_size_; ++i) {
            data_[i] = x.data_[i] * alpha + y.data_[i] * beta;
        }
    }

    template <typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    inline T Norm2() const noexcept {
        T result = NHelpers::InnerProd(data_, data_, mem_size_);
        return std::sqrt(result);
    }

    ~CommonContainer() {
        if (data_ && Hold) {
            delete[] data_;
            data_ = nullptr;
        }
    }

    size_t mem_size_;
    T* data_{nullptr};
};