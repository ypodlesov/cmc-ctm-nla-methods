#include "qr_decomposition.h"

#include <helpers.h>
#include <vector.h>

bool QRDecomposition(TMatrix<double>& Q, TMatrix<double>& R) {
    if (Q.Size1 != Q.Size2) {
        return false;
    }
    const size_t n = Q.Size1;
    R = TMatrix<double>(Q.Size2, Q.Size2);
    R.Nullify();
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < j; ++i) {
            double innerProd = TMatrix<double>::InnerProd(Q, i, Q, j);
            R.Data[j * n + i] = innerProd;
            for (size_t k = 0; k < n; ++k) {
                Q.Data[j * n + k] -= Q.Data[i * n + k] * innerProd;
            }
        } 
        double rjj = Q.ColumnNorm2(j);
        R.Data[j * n + j] = rjj;
        for (size_t k = 0; k < n; ++k) {
            Q.Data[j * n + k] /= rjj;
        }
    }
    return true;
}

bool QRDecompositionBlockOptimized(TMatrix<double>& Q, TMatrix<double>& R) {
    if (Q.Size1 != Q.Size2) {
        return false;
    }
    const size_t blockSize = 8;
    const size_t n = Q.Size1;
    R = TMatrix<double>(n, n);
    R.Nullify();
    for (size_t j = 0; j < n; ++j) {
        double* jthRColumn = R.GetColumn(j);
        double* jthQColumn = Q.GetColumn(j);
        double* innerProdVector = new double[blockSize];
        for (size_t i = 0; i < j; i += blockSize) {
            size_t actualBlockSize = std::min(blockSize, j - i);
            for (size_t k = 0; k < actualBlockSize; ++k) {
                innerProdVector[k] = 0;
            }
            double* ithQColumn = Q.GetColumn(i);
            for (size_t row = 0; row < n; ++row) {
                double coeff = jthQColumn[row];
                for (size_t k = 0; k < actualBlockSize; ++k) {
                    innerProdVector[k] += coeff * ithQColumn[k * n + row];
                }
            }

            for (size_t l = 0; l < actualBlockSize; ++l) {
                jthRColumn[i + l] = innerProdVector[l];
                for (size_t k = 0; k < n; ++k) {
                    jthQColumn[k] -= ithQColumn[l * n + k] * innerProdVector[l];
                }
            }
        }
        delete[] innerProdVector;
        double rjj = 0;
        for (size_t i = 0; i < n; ++i) {
            rjj += jthQColumn[i] * jthQColumn[i];
        }
        rjj = std::sqrt(rjj);
        for (size_t k = 0; k < n; ++k) {
            jthQColumn[k] /= rjj;
        }
        jthRColumn[j] = rjj;
    }
    return true;
}