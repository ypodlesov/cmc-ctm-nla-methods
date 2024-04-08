#pragma once
#include <sparse_matrix.h>
#include <vector.h>

bool ConjugateGradient(const SparseMatrix<double>& a, const Vector<double>& b, Vector<double>& x);