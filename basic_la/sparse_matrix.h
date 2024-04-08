#pragma once
#include "common_matrix.h"
#include <cstddef>
#include <iostream>
#include <random>
#include <set>
#include <utility>

template <typename T>
struct SparseMatrix: public CommonMatrix<SparseMatrix<double>, double> {
    using Base = CommonMatrix<SparseMatrix<double>, double>;

    SparseMatrix() = default;

    SparseMatrix(size_t row_cnt, size_t col_cnt, size_t non_zero)
        : Base(row_cnt, col_cnt, std::min(non_zero, row_cnt * col_cnt))
        , j_a_{new size_t[mem_size_]}
        , i_a_{new size_t[row_cnt]}
    {
        for (size_t i = 0; i < row_cnt; ++i) {
            i_a_[i] = 0;
        }
    }

    SparseMatrix(const SparseMatrix& other)
        : Base(other)
        , j_a_{new size_t[mem_size_]}
        , i_a_{new size_t[row_cnt_]}
    {
        for (size_t i = 0; i < mem_size_; ++i) {
            j_a_[i] = other.j_a_[i];
        }
        for (size_t i = 0; i < row_cnt_; ++i) {
            i_a_[i] = other.i_a_[i];
        }
    }

    SparseMatrix(SparseMatrix&& other)
        : Base(std::move(other))
        , j_a_{other.j_a_}
        , i_a_{other.i_a_}
    {
        other.j_a_ = nullptr;
        other.i_a_ = nullptr;
    }

    SparseMatrix& operator =(const SparseMatrix& other) {
        Base::operator =(other);
        j_a_ = new size_t[mem_size_];
        i_a_ = new size_t[row_cnt_];
        for (size_t i = 0; i < mem_size_; ++i) {
            j_a_[i] = other.j_a_[i];
        }
        for (size_t i = 0; i < row_cnt_; ++i) {
            i_a_[i] = other.i_a_[i];
        }
        return *this;
    }

    SparseMatrix& operator =(SparseMatrix&& other) {
        Base::operator =(std::move(other));
        std::swap(j_a_, other.j_a_);
        other.j_a_ = nullptr;
        std::swap(i_a_, other.i_a_);
        other.i_a_ = nullptr;
        return *this;
    }

    inline T operator()(const size_t row, const size_t col) const {
        T result;
        GetImpl(row, col, result);
        return result;
    }

    inline void GetImpl(const size_t row, const size_t col, T& result) const {
        size_t to_search_start = i_a_[row];
        if (row && !to_search_start) {
            result = {};
            return;
        }
        size_t to_search_end = row == row_cnt_ - 1 ? mem_size_ : i_a_[row + 1];
        for (size_t i = to_search_start; i < to_search_end; ++i) {
            if (j_a_[i] == col) {
                result = data_[i];
                return;
            }
        }
        result = {};
    }

    inline T GetImpl(size_t row, size_t col) const {
        T result;
        GetImpl(row, col, result);
        return result;
    }

    void VecMult(const Vector<T>& x, Vector<T>& result) const {
        assert(result.data_ && result.mem_size_ == row_cnt_ && col_cnt_ == x.mem_size_);
        NHelpers::Nullify(result.data_, result.mem_size_);
        size_t i = 0;
        for (size_t j = 0; j < mem_size_; ++j) {
            if (i + 1 < row_cnt_ && i_a_[i + 1] <= j) {
                ++i;
            }
            result.data_[i] += data_[j] * x.data_[j_a_[j]];
        }
    }


    ~SparseMatrix() {
        if (j_a_) {
            delete[] j_a_;
            j_a_ = nullptr;
        }
        if (i_a_) {
            delete[] i_a_;
            i_a_ = nullptr;
        }
    }

    size_t* j_a_{nullptr};
    size_t* i_a_{nullptr};
};

namespace NHelpers {

    template <typename T>
    void GenRandomMatrix(SparseMatrix<T>& a, [[maybe_unused]] bool is_int = false) {
        assert((a.row_cnt_ * a.col_cnt_) % a.mem_size_ == 0);
        size_t non_zero_per_row = (a.row_cnt_ * a.col_cnt_) / a.mem_size_;
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_real_distribution<T> real_distribution(T{}, a.row_cnt_ * a.col_cnt_);
        std::uniform_int_distribution<size_t> int_distribution(T{}, a.col_cnt_ - 1);
        std::vector<size_t> cols;
        std::vector<T> data;
        for (size_t i = 0; i < a.row_cnt_; ++i) {
            std::set<size_t> non_zero_cols;
            for (size_t j = 0; j < non_zero_per_row; ++j) {
                size_t cur_col = int_distribution(random_engine);
                while (non_zero_cols.contains(cur_col)) {
                    cur_col = int_distribution(random_engine);
                }
                non_zero_cols.insert(cur_col);
            }
            for (const auto& j : non_zero_cols) {
                cols.emplace_back(j);
                data.emplace_back(is_int ? int(real_distribution(random_engine)) : real_distribution(random_engine));
            }
        }
        for (size_t i = 0; i < a.mem_size_; ++i) {
            a.j_a_[i] = cols[i];
            a.data_[i] = data[i];
            if (i % non_zero_per_row == 0) {
                a.i_a_[i / non_zero_per_row] = i;
            }
        }
    }

}

template <typename T>
std::ostream & operator <<(std::ostream &out, const SparseMatrix<T> &sp_matrix) {
    out << sp_matrix.row_cnt_ << ' ' << sp_matrix.col_cnt_ << std::endl;
    out << sp_matrix.mem_size_ << std::endl;
    for (size_t i = 0; i < sp_matrix.mem_size_; ++i) {
        out << sp_matrix.data_[i] << ' ';
    }
    out << std::endl;
    for (size_t i = 0; i < sp_matrix.mem_size_; ++i) {
        out << sp_matrix.j_a_[i] << ' ';
    }
    out << std::endl;
    for (size_t i = 0; i < sp_matrix.row_cnt_; ++i) {
        out << sp_matrix.i_a_[i] << ' ';
    }
    return out;
}

template <typename T>
std::istream & operator >>(std::istream &in,  SparseMatrix<T> &sp_matrix) {
    size_t row_cnt, col_cnt, mem_size;
    in >> row_cnt >> col_cnt >> mem_size;
    sp_matrix = SparseMatrix<T>(row_cnt, col_cnt, mem_size);
    for (size_t i = 0; i < mem_size; ++i) {
        in >> sp_matrix.data_[i];
    }
    for (size_t i = 0; i < mem_size; ++i) {
        in >> sp_matrix.j_a_[i];
    }
    for (size_t i = 0; i < row_cnt; ++i) {
        in >> sp_matrix.i_a_[i];
    }
    assert(sp_matrix.mem_size_ == mem_size && sp_matrix.row_cnt_ == row_cnt && sp_matrix.col_cnt_ == col_cnt);
    return in;
}