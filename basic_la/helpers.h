#pragma once
#include <cmath>
#include <random>
#include <ctime>

namespace NHelpers {

    constexpr double EPS = 1e-9;

    template <class T1, class T2>
    bool RoughEq(T1 lhs, T2 rhs, T2 epsilon = EPS) { 
        return std::fabs(lhs - rhs) < epsilon;
    }

    template <class T1, class T2>
    bool RoughLT(T1 lhs, T2 rhs, T2 epsilon = EPS) {
        return rhs - lhs >= epsilon;
    }

    template <class T1, class T2>
    bool RoughtLTE(T1 lhs, T2 rhs, T2 epsilon = EPS) {
        return rhs - lhs > -epsilon;
    }

    template <typename T>
    void Nullify(T* data_, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            data_[i] = {};
        }
    }

    template <typename T>
    T InnerProd(T* data_1, T* data_2, size_t n) {
        T result{};
        for (size_t i = 0; i < n; ++i) {
            result += data_1[i] * data_2[i];
        }
        return result;
    }

    template <typename T>
    struct Random {

        Random() 
            : gen_{rd()}
        {
        }

        inline T Get() {
            return std::generate_canonical<T, 10>(gen_);
        }

        std::random_device rd;
        std::mt19937 gen_;
    };

    enum class ETriangularType {
        Upper,
        Lower
    };
    
} // namespace NHelpers
