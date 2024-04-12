#pragma once
#include "common_matrix.h"

template <typename T>
struct SparseMatrix: public CommonMatrix<SparseMatrix<double>, double> {
    using Base = CommonMatrix<SparseMatrix<double>, double>;

    SparseMatrix(size_t row_cnt, size_t col_cnt, size_t non_zero)
        : Base(row_cnt, col_cnt, non_zero)
        , non_zero_cnt_{non_zero}
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
        return;
    }

    inline bool Set(const size_t& row, const size_t& col, const T& val) {
        size_t to_search_start = i_a_[row];
        if (row && !to_search_start) {
            return false;
        }
        size_t to_search_end = row == row_cnt_ - 1 ? mem_size_ : i_a_[row + 1];
        T* to_search = j_a_ + i_a_[row];
        for (size_t i = to_search_start; i < to_search_end; ++i) {
            if (to_search[i] == col) {
                data_[i] = val;
                return true;
            }
        }
        return false;
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

namespace NHelpers {

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    void GenRandom(SparseMatrix<T>& matrix, bool is_int = false) {
        std::srand(std::time(nullptr));
        size_t prev_j = matrix.col_cnt_ - 1;
        for (size_t k = 0, i = 0; k < matrix.mem_size_ && i < matrix.row_cnt_; ++k) {
            if (prev_j == matrix.col_cnt_ - 1) {
                prev_j = 0;
                matrix.i_a_[i] = k;
            }
            size_t cur_j = std::min(matrix.col_cnt_ - 1, prev_j + std::abs(std::rand()) % (matrix.col_cnt_ / 2));
            T rnd_elem = matrix.RandElem() * matrix.mem_size_;
            matrix.data_[k] = is_int ? int(rnd_elem) : rnd_elem;
            matrix.j_a_[k] = cur_j;
            prev_j = cur_j;
        }
    }

} // namespace NHelpers