#include "conjugate_gradient.h"
#include "helpers.h"

bool ConjugateGradient(const SparseMatrix<double>& a, const Vector<double>& b, Vector<double>& x) {
    if (!a.data_ || !b.data_) {
        return false;
    }
    if (!x.data_ || x.mem_size_ != a.col_cnt_) {
        x = Vector<double>(a.col_cnt_);
    }
    Vector<double> current_residual = b;
    Vector<double> current_p = b;
    Vector<double> current_x(a.row_cnt_);
    NHelpers::Nullify(current_x.data_, current_x.mem_size_);
    // if (!A || !b || A.Size2 != b.Size) {
    //     return false;
    // }
    // if (!x || x.Size != A.Size2) {
    //     x = TVector<double>(A.Size1);
    // }
    // TVector<double> currentResidual, currentP, currentX;
    // currentP = b;
    // currentResidual = b;
    // currentX = TVector<double>(A.Size1);
    // currentX.Nullify();

    // double current_alpha, current_beta;
    for (size_t j = 0; j < x.mem_size_ && !NHelpers::RoughEq<double, double>(x.Norm2(), 0.0, 0.001); ++j) {
        Vector<double> ap;
        a.VecMult(current_p, ap);
    }

    // for (size_t j = 0; j < x.Size * x.Size && !RoughEq<double, double>(TVector<double>::Norm2(currentResidual), 0, 0.001); ++j) {
    //     TVector<double> Ap;
    //     TMatrix<double>::MVMultiply(A, currentP, Ap);
    //     double innerProduct = TVector<double>::InnerProd(Ap, currentP);
    //     if (RoughEq(innerProduct, 0)) {
    //         x = std::move(currentX);
    //         return true;
    //     }
    //     double currentResidualNorm = TVector<double>::InnerProd(currentResidual, currentResidual);
    //     current_alpha = currentResidualNorm / innerProduct;
    //     currentX = currentX + currentP * current_alpha;
    //     TVector<double> nextResidual = currentResidual - Ap * current_alpha;
    //     if (RoughEq(currentResidualNorm, 0)) {
    //         x = std::move(currentX);
    //         return true; 
    //     }
    //     current_beta = TVector<double>::InnerProd(nextResidual, nextResidual) / currentResidualNorm;
    //     currentP = nextResidual + currentP * current_beta;
    //     currentResidual = nextResidual;
    // }
    // x = std::move(currentX);
    return true;
}