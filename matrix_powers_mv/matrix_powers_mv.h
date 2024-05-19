#pragma once

#include <sparse_matrix.h>
#include <vector.h>

#define IDXTYPEWIDTH 64
#define REALTYPEWIDTH 64

int ReorderMatrix(const SparseMatrix<double>& sp_matrix);
void MatrixPowersMV(const SparseMatrix<double>& sp_matrix, const Vector<double>& x, const Vector<Vector<double>*>& res);