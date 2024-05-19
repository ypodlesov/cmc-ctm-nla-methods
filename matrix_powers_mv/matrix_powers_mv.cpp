#include "matrix_powers_mv.h"

int ReorderMatrix(const SparseMatrix<double>& sp_matrix) {
    SparseMatrix<double> a_no_diag;
    sp_matrix.RemoveDiag(a_no_diag);
    idx_t* perm = new idx_t[a_no_diag.row_cnt_];
    idx_t* iperm = new idx_t[a_no_diag.row_cnt_];
    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    options[METIS_OPTION_NUMBERING] = 0;
    // int METIS_NodeND(idx_t *nvtxs, idx_t *xadj, idx_t *adjncy, idx_t *vwgt,
    //   idx_t *options, idx_t *perm, idx_t *iperm);

    int result = METIS_NodeND(
        &a_no_diag.row_cnt_
        , a_no_diag.i_a_
        , a_no_diag.j_a_
        , nullptr
        , options
        , perm
        , iperm
    );
    return result;
}

void MatrixPowersMV(const SparseMatrix<double>& /* sp_matrix */, const Vector<double>& /* x */, const Vector<Vector<double>*>& /* res */) {
    return;
}