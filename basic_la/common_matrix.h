#pragma once
#include "common_container.h"
#include "helpers.h"
#include "vector.h"
#include <utility>

template <typename SpecMatrix, typename T, bool Hold = true>
struct CommonMatrix: public CommonContainer<CommonMatrix<SpecMatrix, T, Hold>, T, Hold> {
    using Base = CommonContainer<CommonMatrix<SpecMatrix, T, Hold>, T, Hold>;
    using Base::data_;

    CommonMatrix() = default;

    CommonMatrix(size_t row_cnt, size_t col_cnt, size_t mem_size)
        : Base(mem_size)
        , row_cnt_{row_cnt}
        , col_cnt_{col_cnt}
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

    inline T Get(size_t row, size_t col) const {
        T result;
        static_cast<const SpecMatrix*>(this)->GetImpl(row, col, result);
        return result;
    }

    inline void Get(size_t row, size_t col, T& result) const {
        static_cast<const SpecMatrix*>(this)->GetImpl(row, col, result);
    }

    inline size_t GetMemSize() const {
        return static_cast<const SpecMatrix*>(this)->GetMemSizeImpl();
    }

    bool IsTriangular(NHelpers::ETriangularType type) const {
        bool is_upper = type == NHelpers::ETriangularType::Upper;
        bool flag = true;
        const SpecMatrix* mat = static_cast<const SpecMatrix*>(this);
        assert(mat);
        for (size_t i = 0; i < row_cnt_; ++i) {
            for (size_t j = (is_upper ? 0 : i + 1); j < (is_upper ? i : col_cnt_); ++j) {
                flag &= NHelpers::RoughEq(mat->GetImpl(i, j), 0.0);
            }
        }
        return flag;
    }

    void VecMult(const Vector<T>& x, Vector<T>& result) const {
        if (!result.data_ || result.mem_size_ != row_cnt_) {
            result = Vector<T>(row_cnt_);
        }
        NHelpers::Nullify(result.data_, result.mem_size_);
        auto* original = static_cast<const SpecMatrix*>(this);
        assert(original);
        for (size_t j = 0; j < col_cnt_; ++j) {
            T x_j = x.data_[j];
            for (size_t i = 0; i < row_cnt_; ++i) {
                result.data_[i] += original->GetImpl(i, j) * x_j;
            }
        }
    }

    ~CommonMatrix() {
        row_cnt_ = col_cnt_ = 0;
    }

    size_t row_cnt_{};
    size_t col_cnt_{};
};