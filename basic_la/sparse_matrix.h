#pragma once
#include "common_matrix.h"

template <typename T>
struct SparseMatrix: public CommonMatrix<SparseMatrix<double>, double> {
    using Base = CommonMatrix<SparseMatrix<double>, double>;

    SparseMatrix(size_t row_cnt, size_t col_cnt, size_t non_zero)
        : Base(row_cnt, col_cnt, std::min(non_zero, row_cnt * col_cnt))
        , j_a_{new size_t[non_zero_cnt_]}
        , i_a_{new size_t[row_cnt]}
    {
        for (size_t i = 0; i < row_cnt; ++i) {
            i_a_[i] = 0;
        }
    }

    inline T operator()(const size_t& row, const size_t& col) const {
        T result;
        GetImpl(row, col, result);
        return result;
    }

    inline void GetImpl(size_t row, size_t col, T& result) const {
        size_t to_search_start = i_a_[row];
        if (row && !to_search_start) {
            result = {};
            return;
        }
        size_t to_search_end = row == row_cnt_ - 1 ? mem_size_ : i_a_[row + 1];
        size_t* to_search = &j_a_[i_a_[row]];
        for (size_t i = to_search_start; i < to_search_end; ++i) {
            if (to_search[i] == col) {
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


    ~SparseMatrix() {
        non_zero_cnt_ = 0;
        if (j_a_) {
            delete[] j_a_;
            j_a_ = nullptr;
        }
        if (i_a_) {
            delete[] i_a_;
            i_a_ = nullptr;
        }
    }

    size_t non_zero_cnt_;
    size_t* j_a_{nullptr};
    size_t* i_a_{nullptr};
};