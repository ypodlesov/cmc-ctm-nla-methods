#pragma once
#include <matrix.h>
#include <vector.h>

bool ConjugateGradient(const TMatrix<long double>& A, const TVector<long double>& b, TVector<long double>& x);
