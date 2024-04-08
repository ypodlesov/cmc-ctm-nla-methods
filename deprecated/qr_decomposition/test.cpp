#include "qr_decomposition.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using Catch::Generators::random;
using Catch::Generators::take;

static void Test(const uint32_t n) {
    std::srand(std::time(nullptr));
    const TMatrix<double> A = TMatrix<double>::CreateRandom(n, n);
    auto Q = A;
    TMatrix<double> R;
    REQUIRE(QRDecomposition(Q, R));
    TMatrix C(TMatrix<double>::MMMultiply(Q, R));
    REQUIRE(A == C);
    REQUIRE(TMatrix<double>::IsTriangular(R, ETriangularType::Upper));
    REQUIRE(TMatrix<double>::MMMultiply(Q, Q.Transpose()) == TMatrix<double>::CreateIdentityMatrix(n));
}

TEST_CASE("Size 10") {
    Test(10);
}

TEST_CASE("Size 100") {
    Test(100);
}