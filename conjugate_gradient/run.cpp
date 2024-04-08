#include "conjugate_gradient.h"
#include "helpers.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

void Prepare(TMatrix<double>& A, TVector<double>& b, const size_t n) {
    std::srand(std::time(nullptr));
    A = TMatrix<double>::GenerateRandomSymmetricDefinite(n, EDefiniteness::Positive);
    b = TVector<double>::CreateRandom(n);
}

TEST_CASE("Benchmark CG") {
    TMatrix<double> A;
    TVector<double> b;
    TVector<double> x;    
    TVector<double> result;
    {
        Prepare(A, b, 10);
        BENCHMARK("Size 10") {
            ConjugateGradient(A, b, x);
        };
        REQUIRE(TMatrix<double>::MVMultiply(A, x, result));
        REQUIRE(RoughEq<double, double>(TVector<double>::Norm2(result - b), 0, 0.001));
    }
    {
        Prepare(A, b, 500);
        BENCHMARK("Size 500") {
            ConjugateGradient(A, b, x);
        };
        REQUIRE(TMatrix<double>::MVMultiply(A, x, result));
        REQUIRE(RoughEq<double, double>(TVector<double>::Norm2(result - b), 0, 0.001));
    }
    {
        Prepare(A, b, 1000);
        BENCHMARK("Size 1000") {
            ConjugateGradient(A, b, x);
        };
        REQUIRE(TMatrix<double>::MVMultiply(A, x, result));
        REQUIRE(RoughEq<double, double>(TVector<double>::Norm2(result - b), 0, 0.01));
    }
}
