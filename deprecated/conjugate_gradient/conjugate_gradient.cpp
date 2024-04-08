#include "conjugate_gradient.h"
#include "vector.h"
#include <helpers.h>

bool ConjugateGradient(const TMatrix<double>& A, const TVector<double>& b, TVector<double>& x) {
    if (!A || !b || A.Size2 != b.Size) {
        return false;
    }
    if (!x || x.Size != A.Size2) {
        x = TVector<double>(A.Size1);
    }
    TVector<double> currentResidual, currentP, currentX;
    currentP = b;
    currentResidual = b;
    currentX = TVector<double>(A.Size1);
    currentX.Nullify();
    double currentAlpha, currentBeta;
    for (size_t j = 0; j < x.Size * x.Size && !RoughEq<double, double>(TVector<double>::Norm2(currentResidual), 0, 0.001); ++j) {
        TVector<double> Ap;
        TMatrix<double>::MVMultiply(A, currentP, Ap);
        double innerProduct = TVector<double>::InnerProd(Ap, currentP);
        if (RoughEq(innerProduct, 0)) {
            x = std::move(currentX);
            return true;
        }
        double currentResidualNorm = TVector<double>::InnerProd(currentResidual, currentResidual);
        currentAlpha = currentResidualNorm / innerProduct;
        currentX = currentX + currentP * currentAlpha;
        TVector<double> nextResidual = currentResidual - Ap * currentAlpha;
        if (RoughEq(currentResidualNorm, 0)) {
            x = std::move(currentX);
            return true; 
        }
        currentBeta = TVector<double>::InnerProd(nextResidual, nextResidual) / currentResidualNorm;
        currentP = nextResidual + currentP * currentBeta;
        currentResidual = nextResidual;
    }
    x = std::move(currentX);
    return true;
}