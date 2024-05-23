#pragma once
#include <sparse_matrix.h>
#include <matrix.h>
#include <vector.h>

bool ConjugateGradientParallel(const SparseMatrix<double>& a, const Vector<double>& b, Vector<double>& x);