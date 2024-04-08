#include "qr_decomposition.h" 

bool QRDecomposition(SquareMatrix <double> &q, SquareMatrix <double> &r) {
    const size_t n = q.row_cnt_;
    r = SquareMatrix<double>(n);
    NHelpers::Nullify(r.data_, r.mem_size_);
    for (size_t j = 0; j < n; ++j) {
        double *q_j_col = &q.data_[j * n];
        for (size_t i = 0; i < j; ++i) {
            double inner_prod = NHelpers::InnerProd(q_j_col, &q.data_[i * n], n);
            r(i, j) = inner_prod;
            double *q_i_col = &q.data_[i * n];
            for (size_t k = 0; k < n; ++k) {
                q_j_col[k] -= q_i_col[k] * inner_prod;
            }
        }
        double r_jj = NHelpers::InnerProd(q_j_col, q_j_col, n);
        r(j, j) = r_jj;
        for (size_t k = 0; k < n && !NHelpers::RoughEq(r_jj, 0); ++k) {
            q_j_col[k] /= r_jj;
        }
    }
    return true;
}