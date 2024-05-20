#pragma once
#include "helpers.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <type_traits>
#include <utility>

template <typename ContainerType, typename T, bool Hold = true>
struct CommonContainer {

    CommonContainer() = default;

    CommonContainer(int64_t mem_size)
        : mem_size_{mem_size}
        , data_{new T[mem_size]}
    {
    }

    CommonContainer(const CommonContainer& other) {
        mem_size_ = other.mem_size_;
        data_ = new T[mem_size_];
        for (int64_t i = 0; i < mem_size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    CommonContainer(CommonContainer&& other) {
        if (data_ && Hold) {
            delete[] data_;
        }
        data_ = nullptr;
        std::swap(data_, other.data_);
        other.data_ = nullptr;
        std::swap(mem_size_, other.mem_size_);
        other.mem_size_ = 0;
    }

    CommonContainer(T* data, int64_t mem_size) 
        : mem_size_{mem_size}
        , data_{data}
    {
    }

    CommonContainer& operator =(const CommonContainer& other) {
        if (data_ && Hold) {
            delete[] data_;
        }
        data_ = nullptr;
        mem_size_ = other.mem_size_;
        data_ = new T[mem_size_];
        for (int64_t i = 0; i < mem_size_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }

    CommonContainer& operator =(CommonContainer&& other) {
        if (data_ && Hold) {
            delete[] data_;
        }
        data_ = nullptr;
        std::swap(data_, other.data_);
        mem_size_ = 0;
        std::swap(mem_size_, other.mem_size_);
        return *this;
    }

    template <typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    void PlusAX(const CommonContainer& x, const T alpha = 1) {
        assert(mem_size_ == x.mem_size_ && data_ && x.data_);
        for (int64_t i = 0; i < mem_size_; ++i) {
            data_[i] += alpha * x.data_[i];
        }
    }

    template <typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    void AXPlusBY(const CommonContainer& x, const T alpha, const CommonContainer& y, const T beta) {
        assert(mem_size_ == x.mem_size_ && data_ && x.mem_size_ == y.mem_size_);
        for (int64_t i = 0; i < mem_size_; ++i) {
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
        }
        data_ = nullptr;
    }

    int64_t mem_size_{0};
    T* data_{nullptr};
};