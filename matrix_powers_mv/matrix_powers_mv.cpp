#include "matrix_powers_mv.h"
#include <cstdint>
#include <cstring>

bool ReorderMatrix(SparseMatrix<double>& sp_matrix) {
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
        int64_t cur_row_cnt = sp_matrix.i_a_[i + 1] - sp_matrix.i_a_[i];
        std::memcpy(&sp_matrix.data_[sp_matrix.i_a_[i]], &sp_matrix.data_[sp_matrix.i_a_[perm[i]]], cur_row_cnt * sizeof(double));
        std::memcpy(&sp_matrix.j_a_[sp_matrix.i_a_[i]], &sp_matrix.j_a_[sp_matrix.i_a_[perm[i]]], cur_row_cnt * sizeof(int64_t));
        sp_matrix.i_a_[i] = sp_matrix.i_a_[perm[i]];
    }
    return true;
}

bool MatrixPowersMV(SparseMatrix<double>& sp_matrix, const Vector<double>& /* x */, const std::vector<Vector<double>*>& /* res */) {
    if (!ReorderMatrix(sp_matrix)) {
        return false;
    }
    return true;
}