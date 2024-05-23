#pragma once
#include "common_matrix.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <thread>

template <typename T, bool Hold = true>
struct SparseMatrix: public CommonMatrix<SparseMatrix<T, Hold>, T, Hold> {
    using Base = CommonMatrix<SparseMatrix<T, Hold>, T, Hold>;
    using Base::mem_size_;
    using Base::data_;
    using Base::row_cnt_;
    using Base::col_cnt_;

    SparseMatrix() = default;

    SparseMatrix(int64_t row_cnt, const int64_t col_cnt, const int64_t non_zero)
        : Base(row_cnt, col_cnt, std::min(non_zero, row_cnt * col_cnt))
        , j_a_{new int64_t[std::min(non_zero, row_cnt * col_cnt)]}
        , i_a_{new int64_t[row_cnt + 1]}
    {
        std::memset(i_a_, 0, (row_cnt_ + 1) * sizeof(decltype(row_cnt)));
    }

    SparseMatrix(const SparseMatrix& other)
        : Base(other)
        , j_a_{new int64_t[other.mem_size]}
        , i_a_{new int64_t[other.row_cnt + 1]}
    {
        std::memcpy(j_a_, other.j_a_, mem_size_ * sizeof(decltype(col_cnt_)));
        std::memcpy(i_a_, other.i_a_, (row_cnt_ + 1) * sizeof(decltype(row_cnt_)));
    }

    SparseMatrix(SparseMatrix&& other)
        : Base(std::move(other))
        , j_a_{other.j_a_}
        , i_a_{other.i_a_}
    {
        other.j_a_ = nullptr;
        other.i_a_ = nullptr;
    }

    SparseMatrix(const SparseMatrix<T>& other, const int64_t row_start, const int64_t row_end)
        : SparseMatrix{row_end - row_start, other.col_cnt_, std::max<int64_t>(other.i_a_[row_end] - other.i_a_[row_start], 0)}
    {
        std::memcpy(data_, &other.data_[other.i_a_[row_start]], mem_size_ * sizeof(T));
        std::memcpy(j_a_, &other.j_a_[other.i_a_[row_start]], mem_size_ * sizeof(int64_t));
        for (int64_t i = row_start; i < row_end; ++i) {
            i_a_[i - row_start] = std::max<int64_t>(other.i_a_[i] - other.i_a_[row_start], 0);
        }
        i_a_[row_end - row_start] = row_cnt_ * col_cnt_;
    }

    SparseMatrix& operator =(const SparseMatrix& other) {
        Base::operator =(other);
        j_a_ = new int64_t[mem_size_];
        i_a_ = new int64_t[row_cnt_ + 1];
        std::memcpy(j_a_, other.j_a_, mem_size_ * sizeof(decltype(col_cnt_)));
        std::memcpy(i_a_, other.i_a_, (row_cnt_ + 1) * sizeof(decltype(row_cnt_)));
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

    void RemoveDiag(SparseMatrix& result) const {
        int64_t diag_elems_num = 0;
        int64_t i_a_idx = 0;
        int64_t j_a_idx = 0;
        int64_t* tmp_res_i_a = new int64_t[row_cnt_];
        int64_t* tmp_res_j_a = new int64_t[mem_size_];
        T* tmp_res_data = new T[mem_size_];
        std::memset(tmp_res_i_a, 0, (row_cnt_ + 1) * sizeof(decltype(i_a_idx)));
        std::memset(tmp_res_j_a, 0, mem_size_ * sizeof(decltype(i_a_idx)));
        while (i_a_idx < row_cnt_ && j_a_idx < mem_size_) {
            if (i_a_[i_a_idx + 1] <= j_a_idx) {
                ++i_a_idx;
                tmp_res_i_a[i_a_idx] = i_a_[i_a_idx] - diag_elems_num;
            }
            if (i_a_idx == j_a_[j_a_idx]) {
                ++diag_elems_num;
            } else {
                tmp_res_j_a[j_a_idx - diag_elems_num] = j_a_[j_a_idx];
                tmp_res_data[j_a_idx - diag_elems_num] = data_[j_a_idx];
            }
            ++j_a_idx;
        }
        tmp_res_i_a[row_cnt_] = mem_size_ - diag_elems_num;
        result = SparseMatrix<T>(row_cnt_, col_cnt_, mem_size_ - diag_elems_num);
        result.i_a_ = tmp_res_i_a;
        std::memcpy(result.j_a_, tmp_res_j_a, result.mem_size_ * sizeof(decltype(i_a_idx)));
        std::memcpy(result.data_, tmp_res_data, result.mem_size_ * sizeof(T));
    }

    inline T operator()(const int64_t row, const int64_t col) const {
        T result;
        GetImpl(row, col, result);
        return result;
    }

    inline void GetImpl(const int64_t row, const int64_t col, T& result) const {
        int64_t to_search_start = i_a_[row];
        if (row && !to_search_start) {
            result = {};
            return;
        }
        int64_t to_search_end = i_a_[row + 1];
        for (int64_t i = to_search_start; i < to_search_end; ++i) {
            if (j_a_[i] == col) {
                result = data_[i];
                return;
            }
        }
        result = {};
    }

    inline T GetImpl(int64_t row, int64_t col) const {
        T result;
        GetImpl(row, col, result);
        return result;
    }

    void VecMult(const Vector<T>& x, Vector<T>& result, const int64_t row_start = 0, int64_t row_end = 0) const {
        if (row_end == 0) {
            row_end = row_cnt_;
        }
        assert(result.data_ && result.mem_size_ == row_cnt_ && col_cnt_ == x.mem_size_);
        NHelpers::Nullify(&result.data_[row_start], row_end - row_start);
        int64_t i = row_start;
        for (int64_t j = i_a_[row_start]; j < i_a_[row_end]; ++j) {
            if (i + 1 < row_cnt_ && i_a_[i + 1] <= j) {
                ++i;
            }
            result.data_[i] += data_[j] * x.data_[j_a_[j]];
        }
    }

    ~SparseMatrix() {
        if (j_a_ && Hold) {
            delete[] j_a_;
        }
        j_a_ = nullptr;
        if (i_a_ && Hold) {
            delete[] i_a_;
        }
        i_a_ = nullptr;
    }

    int64_t* j_a_{nullptr};
    int64_t* i_a_{nullptr};
};

namespace NHelpers {

    template <typename T>
    void GenRandomMatrix(SparseMatrix<T>& a, [[maybe_unused]] bool is_int = false) {
        assert((a.row_cnt_ * a.col_cnt_) % a.mem_size_ == 0);
        int64_t non_zero_per_row = (a.row_cnt_ * a.col_cnt_) / a.mem_size_;
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_real_distribution<T> real_distribution(T{}, a.row_cnt_ * a.col_cnt_);
        std::uniform_int_distribution<int64_t> int_distribution(T{}, a.col_cnt_ - 1);
        std::vector<int64_t> cols;
        std::vector<T> data;
        for (int64_t i = 0; i < a.row_cnt_; ++i) {
            std::set<int64_t> non_zero_cols;
            for (int64_t j = 0; j < non_zero_per_row; ++j) {
                int64_t cur_col = int_distribution(random_engine);
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
        for (int64_t i = 0; i < a.mem_size_; ++i) {
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
    for (int64_t i = 0; i < sp_matrix.mem_size_; ++i) {
        out << sp_matrix.data_[i] << ' ';
    }
    out << std::endl;
    for (int64_t i = 0; i < sp_matrix.mem_size_; ++i) {
        out << sp_matrix.j_a_[i] << ' ';
    }
    out << std::endl;
    for (int64_t i = 0; i <= sp_matrix.row_cnt_; ++i) {
        out << sp_matrix.i_a_[i] << ' ';
    }
    return out;
}

template <typename T>
std::istream & operator >>(std::istream &in,  SparseMatrix<T> &sp_matrix) {
    int64_t row_cnt, col_cnt, mem_size;
    in >> row_cnt >> col_cnt >> mem_size;
    sp_matrix = SparseMatrix<T>(row_cnt, col_cnt, mem_size);
    for (int64_t i = 0; i < mem_size; ++i) {
        in >> sp_matrix.data_[i];
    }
    for (int64_t i = 0; i < mem_size; ++i) {
        in >> sp_matrix.j_a_[i];
    }
    for (int64_t i = 0; i <= row_cnt; ++i) {
        in >> sp_matrix.i_a_[i];
    }
    assert(sp_matrix.mem_size_ == mem_size && sp_matrix.row_cnt_ == row_cnt && sp_matrix.col_cnt_ == col_cnt);
    return in;
}