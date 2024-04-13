#include "conjugate_gradient.h"
#include "helpers.h"
#include "matrix.h"
#include "vector.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

static void Test(const uint32_t n) {
    std::srand(std::time(nullptr));
    const TMatrix<double> A = TMatrix<double>::GenerateRandomSymmetricDefinite(n, EDefiniteness::Positive);
    const TVector<double> b = TVector<double>::CreateRandom(n);
    TVector<double> x;
    ConjugateGradient(A, b, x);
    TVector<double> result;
    REQUIRE(TMatrix<double>::MVMultiply(A, x, result));
    REQUIRE(RoughEq<double, double>(TVector<double>::Norm2(result - b), 0, 0.001));
}

TEST_CASE("Size 10") {
    Test(10);
}

TEST_CASE("Size 100") {
    Test(100);
}

TEST_CASE("Size 500") {
    Test(500);
}