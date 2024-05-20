#pragma once

#include <metis.h>
#include "sparse_matrix.h"
#include "vector.h"

bool ReorderMatrix(const SparseMatrix<double>& sp_matrix, SparseMatrix<double>& res_matrix);
bool MatrixPowersMV(const SparseMatrix<double>& sp_matrix, const Vector<double>& x, std::vector<Vector<double>>& res);