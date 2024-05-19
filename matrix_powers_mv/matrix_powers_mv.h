#pragma once

#include <metis.h>
#include <sparse_matrix.h>
#include <vector.h>

bool ReorderMatrix(SparseMatrix<double>& sp_matrix);
bool MatrixPowersMV(SparseMatrix<double>& sp_matrix, const Vector<double>& x, const std::vector<Vector<double>*>& res);