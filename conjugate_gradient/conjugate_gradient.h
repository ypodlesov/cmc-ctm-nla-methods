#pragma once
#include <sparse_matrix.h>
#include <matrix.h>
#include <vector.h>

bool ConjugateGradient(const SparseMatrix<double>& a, const Vector<double>& b, Vector<double>& x);
bool ConjugateGradient(const Matrix<double>& a, const Vector<double>& b, Vector<double>& x);