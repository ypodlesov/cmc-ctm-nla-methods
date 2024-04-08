#pragma once
#include "common_matrix.h"
#include "helpers.h"
#include <cmath>
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

    Matrix(const Matrix& other)
        : Base(other)
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
                c_j_col[i] = 0;
                for (size_t k = 0; k < a.col_cnt_; ++k) {
                    c_j_col[i] += a.data_[k * m + i] * b_j_col[k];
                }
            }
        }
    }

    void VecMult(const Vector<T>& x, Vector<T>& result) const {
        assert(result.data_ && result.mem_size_ == row_cnt_ && col_cnt_ == x.mem_size_);
        NHelpers::Nullify(result.data_, result.mem_size_);
        for (size_t j = 0; j < col_cnt_; ++j) {
            T x_j = x.data_[j];
            T* j_col = &data_[j * row_cnt_];
            for (size_t i = 0; i < row_cnt_; ++i) {
                result.data_[i] += j_col[i] * x_j;
            }
        }
    }
};

template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
bool operator ==(const Matrix<T>& a, const Matrix<T>& b) {
    size_t m = a.row_cnt_;
    size_t n = a.col_cnt_;
    if (m != b.row_cnt_ || n != b.col_cnt_) {
        return false;
    }
    double diff = 0;
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < m; ++i) {
            double tmp = std::fabs(a.data_[j * m + i] - b.data_[j * m + i]);
            diff += tmp * tmp;
        }
    }
    return NHelpers::RoughEq<double, double>(std::sqrt(diff), 0.0, 0.000001);
}

namespace NHelpers {

    enum class TriangularType {
        Upper,
        Lower
    };

    enum class Definiteness {
        Positive,
        Negative
    };

    template <typename T>
    void FillRandomMatrix(Matrix<T>& a, bool is_int = false) {
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_real_distribution<T> distribution(0, a.row_cnt_ * a.col_cnt_);
        for (size_t i = 0; i < a.mem_size_; ++i) {
            a.data_[i] = is_int ? int(distribution(random_engine)) : distribution(random_engine);
        }
    }
    
    template <typename T>
    void GenRandomMatrix(Matrix<T>& a, size_t row_cnt, size_t col_cnt, bool is_int = false) {
        a = Matrix<T>(row_cnt, col_cnt);
        FillRandomMatrix(a, is_int);
    }

    template <typename T>
    void FillRandomTriangularMatrix(Matrix<T>& a, TriangularType type, bool is_int = false) {
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_real_distribution<T> distribution(0, a.row_cnt_ * a.col_cnt_);
        for (size_t j = 0; j < a.col_cnt_; ++j) {
            if (type == TriangularType::Upper) {
                for (size_t i = 0; i <= j; ++i) {
                    a.data_[j * a.row_cnt_ + i] = is_int ? int(distribution(random_engine)) : distribution(random_engine);
                }
            } else {
                for (size_t i = j; i < a.row_cnt_; ++i) {
                    a.data_[j * a.row_cnt_ + i] = is_int ? int(distribution(random_engine)) : distribution(random_engine);
                }
            }
        }
    }

    template <typename T>
    void GenRandomTriangularMatrix(Matrix<T>& a, size_t row_cnt, size_t col_cnt, TriangularType type, bool is_int = false) {
        a = Matrix<T>(row_cnt, col_cnt);
        FillRandomTriangularMatrix(a, type, is_int);
    }

    template <typename T>
    void FillRandomSymmetricMatrix(Matrix<T>& a, bool is_int = false) {
        FillRandomTriangularMatrix(a, TriangularType::Upper, is_int);
        for (size_t j = 0; j < a.col_cnt_; ++j) {
            for (size_t i = j + 1; i < a.row_cnt_; ++i) {
                a.data_[j * a.row_cnt_ + i] = a.data_[i * a.row_cnt_ + j];
            }
        }
    }

    template <typename T>
    void GenRandomSymmetricMatrix(Matrix<T>& a, size_t row_cnt, size_t col_cnt, bool is_int = false) {
        a = Matrix<T>(row_cnt, col_cnt);
        FillRandomSymmetricMatrix(a, is_int);
    }
    
    template <typename T>
    void FillRandomDefiniteMatrix(Matrix<T>& a, Definiteness definiteness, bool is_int = false) {
        FillRandomSymmetricMatrix(a, is_int);
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_real_distribution<T> distribution(T{}, a.row_cnt_ * a.col_cnt_);
        for (size_t i = 0; i < a.row_cnt_; ++i) {
            a.data_[i * a.row_cnt_ + i] = definiteness == Definiteness::Positive
                ? std::fabs(a.data_[i * a.row_cnt_ + i]) + 1
                : -std::fabs(a.data_[i * a.row_cnt_ + i]) - 1;
        }
    }

    template <typename T>
    void GenRandomDefiniteMatrix(Matrix<T>& a, size_t row_cnt, size_t col_cnt, Definiteness type, bool is_int = false) {
        a = Matrix<T>(row_cnt, col_cnt);
        FillRandomDefiniteMatrix(a, type, is_int);
    }

} // namespace NHelpers

template <typename T>
std::istream & operator >>(std::istream &in,  Matrix<T> &matrix) {
    size_t sz1, sz2;
    in >> sz1 >> sz2;
    matrix = Matrix<T>(sz1, sz2);
    for (size_t i = 0; i < sz1; ++i) {
        for (size_t j = 0; j < sz2; ++j) {
            in >> matrix.data_[j * sz1 + i];
        }
    }
    return in;
}