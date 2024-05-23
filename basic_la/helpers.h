#pragma once
#include <cmath>
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
    void Nullify(T* data_, int64_t n) {
        for (int64_t i = 0; i < n; ++i) {
            data_[i] = 0;
        }
    }

    template <typename T>
    T InnerProd(T* data_1, T* data_2, int64_t n, T& result) {
        for (int64_t i = 0; i < n; ++i) {
            result += data_1[i] * data_2[i];
        }
        return result;
    }

    template <typename T>
    T InnerProd(T* data_1, T* data_2, int64_t n) {
        T result{};
        for (int64_t i = 0; i < n; ++i) {
            result += data_1[i] * data_2[i];
        }
        return result;
    }

    enum class ETriangularType {
        Upper,
        Lower
    };

} // namespace NHelpers
