#pragma once
#include "common_container.h"
#include "helpers.h"
#include "vector.h"
#include <cassert>
#include <cstdint>
#include <utility>

template <typename SpecMatrix, typename T, bool Hold = true>
struct CommonMatrix: public CommonContainer<CommonMatrix<SpecMatrix, T, Hold>, T, Hold> {
    using Base = CommonContainer<CommonMatrix<SpecMatrix, T, Hold>, T, Hold>;
    using Base::data_;

    CommonMatrix() = default;

    CommonMatrix(int64_t row_cnt, int64_t col_cnt, int64_t mem_size)
        : Base(mem_size)
        , row_cnt_{row_cnt}
        , col_cnt_{col_cnt}
    {
    }

    CommonMatrix(const CommonMatrix& other)
        : Base(other)
        , row_cnt_{other.row_cnt_}
        , col_cnt_{other.col_cnt_}
    {
    }

    CommonMatrix(CommonMatrix&& other)
        : Base(std::move(other))
        , row_cnt_{other.row_cnt_}
        , col_cnt_{other.col_cnt_}
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

    CommonMatrix(T* data, const int64_t mem_size, const int64_t row_cnt, const int64_t col_cnt)
        : Base{data, mem_size}
        , row_cnt_{row_cnt}
        , col_cnt_{col_cnt}
    {
    }

    inline T Get(int64_t row, int64_t col) const {
        T result;
        static_cast<const SpecMatrix*>(this)->GetImpl(row, col, result);
        return result;
    }

    inline void Get(int64_t row, int64_t col, T& result) const {
        static_cast<const SpecMatrix*>(this)->GetImpl(row, col, result);
    }

    inline int64_t GetMemSize() const {
        return static_cast<const SpecMatrix*>(this)->GetMemSizeImpl();
    }

    bool IsTriangular(NHelpers::ETriangularType type) const {
        bool is_upper = type == NHelpers::ETriangularType::Upper;
        bool flag = true;
        const SpecMatrix* mat = static_cast<const SpecMatrix*>(this);
        assert(mat);
        for (int64_t i = 0; i < row_cnt_; ++i) {
            for (int64_t j = (is_upper ? 0 : i + 1); j < (is_upper ? i : col_cnt_); ++j) {
                flag &= NHelpers::RoughEq(mat->GetImpl(i, j), 0.0);
            }
        }
        return flag;
    }

    ~CommonMatrix() {
        row_cnt_ = col_cnt_ = 0;
    }

    int64_t row_cnt_{};
    int64_t col_cnt_{};
};