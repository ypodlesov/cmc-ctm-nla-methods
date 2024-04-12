#pragma once
#include "matrix.h"

template <typename T, bool Hold = true>
struct SquareMatrix: public Matrix<T, Hold> {
    using Base = Matrix<T>;

    SquareMatrix(size_t size = 0)
        : Base(size, size)
    {
    }

    SquareMatrix(SquareMatrix&& other)
        : Base{std::move(other)}
    {
    }

    SquareMatrix& operator =(const SquareMatrix& other) {
        Base::operator =(other);
        return *this;
    }

    SquareMatrix& operator =(SquareMatrix&& other) {
        Base::operator =(std::move(other));
        return *this;
    }

};