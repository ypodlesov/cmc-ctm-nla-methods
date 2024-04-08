#pragma once
#include "common.h"
#include "helpers.h"
#include <utility>
#include <cmath>

template <typename SpecMatrix, typename T, bool Hold = true>
struct CommonMatrix: public CommonContainer<CommonMatrix<SpecMatrix, T, Hold>, T, Hold> {
    using Base = CommonContainer<CommonMatrix<SpecMatrix, T, Hold>, T, Hold>;
    using Base::data_;

    CommonMatrix(size_t row_cnt = 0, size_t col_cnt = 0)
        : Base(SpecMatrix::GetMemSize(row_cnt, col_cnt))
        , row_cnt_{row_cnt}
        , col_cnt_{col_cnt}
    {
    }

    CommonMatrix(CommonMatrix&& other)
        : Base(std::move(other))
    {
    }

    CommonMatrix& operator =(const CommonMatrix& other) {
        Base::operator =(other);
        row_cnt_ = other.row_cnt_;
        col_cnt_ = other.col_cnt_;
        return *this;
    }

    CommonMatrix& operator =(CommonMatrix&& other) {
        Base::operator =(std::move(other));
        std::swap(row_cnt_, other.row_cnt_);
        std::swap(col_cnt_, other.col_cnt_);
        return *this;
    }

    inline T Get(size_t row, size_t col) {
        T result;
        SpecMatrix::GetImpl(data_, row_cnt_, row, col, result);
        return result;
    }

    inline void Get(size_t row, size_t col, T& result) {
        SpecMatrix::GetImpl(data_, row_cnt_, row, col, result);
    }

    inline static size_t GetMemSize(size_t row_cnt, size_t col_cnt) {
        return SpecMatrix::GetMemSizeImpl(row_cnt, col_cnt);
    }

    bool IsTriangular(NHelpers::ETriangularType type) {
        bool is_upper = type == NHelpers::ETriangularType::Upper;
        bool flag = true;
        for (size_t i = 0; i < row_cnt_; ++i) {
            for (size_t j = (is_upper ? 0 : i + 1); j < (is_upper ? i : col_cnt_); ++j) {
                flag &= NHelpers::RoughEq(Get(i, j), 0.0);
            }
        }
        return flag;
    }

    ~CommonMatrix() {
        row_cnt_ = col_cnt_ = 0;
    }

    size_t row_cnt_{};
    size_t col_cnt_{};
};

template <typename T, bool Hold = true>
struct Matrix: public CommonMatrix<Matrix<T>, T, Hold> {
    using Base = CommonMatrix<Matrix, T, Hold>;
    using Base::data_;
    using Base::row_cnt_;

    Matrix(size_t row_cnt = 0, size_t col_cnt = 0) 
        : Base(row_cnt, col_cnt)
    {
    }

    Matrix(Matrix&& other)
        : Base(std::move(other))
    {
    }

    Matrix& operator =(const Matrix& other) {
        Base::operator =(other);
        return *this;
    }

    Matrix& operator =(Matrix&& other) {
        Base::operator =(std::move(other));
        return *this;
    }

    inline T& operator ()(size_t row, size_t col) {
        return data_[col * row_cnt_ + row];
    }
    
    inline static size_t GetMemSizeImpl(size_t row_cnt, size_t col_cnt) {
        return row_cnt * col_cnt;
    }

    inline static void GetImpl(T* data, size_t row_cnt, size_t row, size_t col, T& result) {
        if (!data) {
            return;
        }
        result = data[col * row_cnt + row];
    }

    static void MMMult(const Matrix& a, const Matrix& b, Matrix& c) {
        if (a.col_cnt_ != b.row_cnt_) {
            return;
        }
        size_t m = a.row_cnt_;
        size_t n = b.col_cnt_;
        c = Matrix(m, n);
        for (size_t j = 0; j < n; ++j) {
            T* c_j_col = &c.data_[j * c.row_cnt_];
            for (size_t i = 0; i < m; ++i) {
                T* b_j_col = &b.data_[j * b.row_cnt_];
                c_j_col[i] = {};
                for (size_t k = 0; k < a.col_cnt_; ++k) {
                    c_j_col[i] += a.data_[k * m + i] * b_j_col[k];
                }
            }
        }
    }
};

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

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
bool operator ==(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.mem_size_ != b.mem_size_) {
        return false;
    }
    T diff{};
    for (size_t i = 0; i < a.mem_size_; ++i) {
        diff += std::fabs(a.data_[i] - b.data_[i]);
    }
    return NHelpers::RoughEq<double>(diff, 0.0, 0.0001);
}