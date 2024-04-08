#pragma once
#include "helpers.h"
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <random>
#include <type_traits>
#include <utility>
#include <cassert>

template <typename ContainerType, typename T, bool Hold = true>
struct CommonContainer {

    CommonContainer(size_t mem_size)
        : mem_size_{mem_size}
        , data_{new T[mem_size_]}
        , random_ptr_{new NHelpers::Random<T>()}
    {
    }

    CommonContainer(CommonContainer&& other) {
        std::swap(data_, other.data_);
        other.data_ = nullptr;
        std::swap(mem_size_, other.mem_size_);
        other.mem_size_ = 0;
        std::swap(random_ptr_, other.random_ptr_);
        other.random_ptr_ = nullptr;
    }

    CommonContainer& operator =(const CommonContainer& other) {
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
        std::swap(random_ptr_, other.random_ptr_);
        other.random_ptr_ = nullptr;
        return *this;
    }

    template <typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    void GenRandom(size_t n, bool is_int = false) {
        for (size_t i = 0; i < n; ++i) {
            data_[i] = RandElem() * n * n;
            if (is_int) {
                data_[i] = int(data_[i]);
            }
        }
    }

    T RandElem() const {
        return random_ptr_->Get();
    }

    ~CommonContainer() {
        if (Hold && data_) {
            delete[] data_;
            data_ = nullptr;
        }
    }

    size_t mem_size_;
    T* data_{nullptr};
    NHelpers::Random<T>* random_ptr_{nullptr};
};

