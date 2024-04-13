#pragma once
#include "common_matrix.h"
#include "helpers.h"
#include <random>

template <typename T, bool Hold = true>
struct Matrix: public CommonMatrix<Matrix<T>, T, Hold> {
    using Base = CommonMatrix<Matrix, T, Hold>;
    using Base::data_;
    using Base::row_cnt_;
    using Base::col_cnt_;

    Matrix() = default;

    Matrix(size_t row_cnt, size_t col_cnt) 
        : Base(row_cnt, col_cnt, row_cnt * col_cnt)
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
    
    inline size_t GetMemSizeImpl() const {
        return row_cnt_ * col_cnt_;
    }
    
    inline static size_t GetMemSize(const size_t& row_cnt, const size_t& col_cnt) {
        return row_cnt * col_cnt;
    }

    inline T GetImpl(size_t row, size_t col) const {
        assert(data_);
        T result;
        GetImpl(row, col, result);
        return result;
    }

    inline void GetImpl(size_t row, size_t col, T& result) const {
        assert(data_);
        result = data_[col * row_cnt_ + row];
    }

    static void MMMult(const Matrix& a, const Matrix& b, Matrix& c) {
        assert(a.col_cnt_ == b.row_cnt_);
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

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
bool operator ==(const Matrix<T>& a, const Matrix<T>& b) {
    Matrix<T> tmp;
    tmp.AXPlusBY(a, 1.0, b, -1.0);
    return NHelpers::RoughEq<double, double>(tmp.Norm2(), 0.0, 0.000001);
    // if (a.mem_size_ != b.mem_size_) {
    //     return false;
    // }
    // T diff{};
    // for (size_t i = 0; i < a.mem_size_; ++i) {
    //     diff += std::fabs(a.data_[i] - b.data_[i]);
    // }
    // return NHelpers::RoughEq<double>(diff, 0.0, 0.001);
}

namespace NHelpers {
    
    template <typename T>
    void GenRandomMatrix(Matrix<T>& a, size_t row_cnt, size_t col_cnt, [[maybe_unused]] bool is_int = false) {
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_real_distribution<T> distribution(T{}, row_cnt * col_cnt);
        a = Matrix<T>(row_cnt, col_cnt);
        for (size_t i = 0; i < a.mem_size_; ++i) {
            a.data_[i] = distribution(random_engine);
        }
    }

} // namespace NHelpers