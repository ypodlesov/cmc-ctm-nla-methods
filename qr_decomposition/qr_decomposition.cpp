#include "qr_decomposition.h" 

bool QRDecomposition(Matrix<double>& q, Matrix<double>& r) {
    const size_t n = q.row_cnt_;
    r = Matrix<double>(n, n);
    NHelpers::Nullify(r.data_, r.mem_size_);
    for (size_t j = 0; j < n; ++j) {
        double *q_j_col = &q.data_[j * n];
        for (size_t i = 0; i < j; ++i) {
            double *q_i_col = &q.data_[i * n];
            double inner_prod = NHelpers::InnerProd(q_j_col, q_i_col, n);
            r(i, j) = inner_prod;
            for (size_t k = 0; k < n; ++k) {
                q_j_col[k] -= q_i_col[k] * inner_prod;
            }
        }
        double r_jj = NHelpers::InnerProd(q_j_col, q_j_col, n);
        r(j, j) = r_jj;
        for (size_t k = 0; k < n && !NHelpers::RoughEq<double, double>(r_jj, 0, 0.000001); ++k) {
            q_j_col[k] /= r_jj;
        }
    }
    return true;
}