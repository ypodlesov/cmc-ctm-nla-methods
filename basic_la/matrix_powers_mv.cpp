#include "matrix_powers_mv.h"
#include "helpers.h"
#include "sparse_matrix.h"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <thread>

bool ReorderMatrix(const SparseMatrix<double>& sp_matrix, SparseMatrix<double>& res_matrix) {
    SparseMatrix<double> a_no_diag;
    sp_matrix.RemoveDiag(a_no_diag);

    idx_t* perm = new idx_t[a_no_diag.row_cnt_];
    idx_t* iperm = new idx_t[a_no_diag.row_cnt_];
    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    options[METIS_OPTION_NUMBERING] = 0;

    int result = METIS_NodeND(
        &a_no_diag.row_cnt_
        , a_no_diag.i_a_
        , a_no_diag.j_a_
        , nullptr
        , options
        , perm
        , iperm
    );

    if (result != METIS_OK) {
        return false;
    }
    for (int64_t i = 0; i < sp_matrix.row_cnt_; ++i) {
        int64_t cur_row_cnt = sp_matrix.i_a_[perm[i] + 1] - sp_matrix.i_a_[perm[i]];
        std::memcpy(&res_matrix.data_[sp_matrix.i_a_[i]], &sp_matrix.data_[sp_matrix.i_a_[perm[i]]], cur_row_cnt * sizeof(double));
        std::memcpy(&res_matrix.j_a_[sp_matrix.i_a_[i]], &sp_matrix.j_a_[sp_matrix.i_a_[perm[i]]], cur_row_cnt * sizeof(int64_t));
        res_matrix.i_a_[i] = sp_matrix.i_a_[perm[i]];
    }
    return true;
}


bool MatrixPowersMV(const SparseMatrix<double>& sp_matrix, const Vector<double>& x, std::vector<Vector<double>>& res) {
    res.front() = x;

    const int64_t cores_num = std::min<int64_t>(x.mem_size_, std::thread::hardware_concurrency());
    assert(sp_matrix.row_cnt_ % cores_num == 0);
    const int64_t row_step = sp_matrix.row_cnt_ / cores_num;
    std::vector<std::pair<SparseMatrix<double>, Vector<double>>> matrix_blocks(cores_num);
    {
        int64_t cur_row_start = 0;
        for (auto& [matrix_block, vector_block] : matrix_blocks) {
            if (sp_matrix.i_a_[cur_row_start + row_step] - sp_matrix.i_a_[cur_row_start] <= 0) {
                continue;
            }
            vector_block = Vector<double>(row_step);
            matrix_block = SparseMatrix<double>(sp_matrix, cur_row_start, cur_row_start + row_step);
            cur_row_start += row_step;
        }
    }

    for (auto cur_x_iter = std::next(res.begin()); cur_x_iter != res.end(); ++cur_x_iter) {
        auto& prev_x = *std::prev(cur_x_iter);
        auto& cur_x = *cur_x_iter;
        assert(prev_x.mem_size_ == x.mem_size_);
        assert(cur_x.mem_size_ == x.mem_size_);
        NHelpers::Nullify(cur_x.data_, cur_x.mem_size_);
        
        {
            std::vector<std::thread> threads;
            threads.reserve(cores_num);
            {
                int64_t cur_row_start = 0;
                for (auto& [matrix_block, vector_block] : matrix_blocks) {
                    threads.emplace_back([&, cur_row_start, row_step]() {
                        matrix_block.VecMult(prev_x, vector_block);
                        for (int64_t i = cur_row_start; i < cur_row_start + row_step; ++i) {
                            cur_x.data_[i] = vector_block.data_[i - cur_row_start];
                        }
                    });
                    cur_row_start += row_step;
                }
                for (auto&& thread : threads) {
                    thread.join();
                }
            }
        }
    }
    return true;
}