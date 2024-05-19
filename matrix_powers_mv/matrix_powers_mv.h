#pragma once

#include <metis.h>
#include <sparse_matrix.h>
#include <vector.h>

int ReorderMatrix(const SparseMatrix<double>& sp_matrix);
void MatrixPowersMV(const SparseMatrix<double>& sp_matrix, const Vector<double>& x, const Vector<Vector<double>*>& res);