#pragma once
#include <cmath>

#define EPS 1e-9

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